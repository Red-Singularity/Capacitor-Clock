// Capacitor Clock main code
// Started 1/24/2026
// communicates  with a screen, an encoder, some buttons, a stepper motor, and some power circuitry
// to explode a small capacitor when the set time has been reached

#include <LiquidCrystal_I2C.h> // include I2C library
#include <AccelStepper.h> // using accel stepper library for motor positioning
#include <DS3231.h> // include library for RTC module
#include <Wire.h>
#include <RotaryEncoder.h> // include library for rotary encoder

#define MotorInterfaceType 4 // defines stepper as a 4 wire
#define I_sense 36 // current sense pin
#define Step1 32 // stepper output 1
#define Step2 33 // stepper output 2
#define Step3 25 // stepper output 3
#define Step4 26 // stepper output 4
#define Button_1 13 // generic button input 1
#define Button_2 14 // generic button input 2
#define LED 19 // logic pin for LED output
#define Fan 18 // fan control pin
#define SW 17 // encoder Button Input
#define DT 16 // encoder DT input
#define CLK 4 // encoder clock input

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper myStepper(MotorInterfaceType, Step1, Step3, Step2, Step4);

LiquidCrystal_I2C lcd(0x27,20,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

DS3231 myRTC; // create instance of the real time clock

RotaryEncoder rotary_encoder(DT, CLK, RotaryEncoder::LatchMode::TWO03); // Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:

enum encoder{clockwise, counter_clockwise, nothing};  // clock wise is interpreted as 0, counter clockwise as 1, nothing as 3

bool century = false;
bool h12Flag = true;
bool pmFlag = false;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;
int menu_location = 0; // total different adjustable fields for clock display

const int stepsPerRevolution = 2048;
unsigned long settings_time = 0;
unsigned long settings_time_old = 0;

void setup() {
  // serial setup for debugging
  Serial.begin(115200);

  Wire.begin(); // start I2C interface

  //pinmode setup
  pinMode(Step1, OUTPUT);
  pinMode(Step2, OUTPUT);
  pinMode(Step3, OUTPUT);
  pinMode(Step4, OUTPUT);
  pinMode(Button_1, INPUT);
  pinMode(Button_2, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(Fan, OUTPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(DT, INPUT);
  pinMode(CLK, INPUT);


  //myRTC.setClockMode(h12Flag); // uploads 'true' (1) to bit 6 of register 0x02
  // set default date to 1/1/2026
  //myRTC.setYear(26); // uploads 26 to register 0x06 for year
  //myRTC.setMonth(1); // uploads 1 to register 0x05 for month
  //myRTC.setDate(1); // uploads 1 to register 0x04 for day
  // set default time to midnight
  //myRTC.setSecond(0); // uploads 0 to register 0x00
  //myRTC.setMinute(0); // uploads 0 to register 0x01
  //myRTC.setHour(24); // uploads 24 to register 0x02

  //stepper setup
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(50.0);
  myStepper.setSpeed(200);
  myStepper.moveTo(2048);

  // LCD setup
  lcd.init(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print(" Cap Clock V1");
  lcd.setCursor(0,1);
  lcd.print("  By Mal & Dustin");
  delay(2000);
  displayFormat(); // display the main format to the display

}

void loop() {
  encoder rotation; // create variable rotation as part of encoder enumerated type 
  static int settings; // create settings flag

  bool CenturyBit; // this bit is required as an input to the month function and cannot be input as a true or false to the function directly
  int Year = 2000 + myRTC.getYear();
  int Month = myRTC.getMonth(CenturyBit);
  int Day = myRTC.getDate();
  int Hour = myRTC.getHour(h12Flag, pmFlag);
  int Minute = myRTC.getMinute();
  int Second = myRTC.getSecond();
  int Encoder_button = digitalRead(SW);

  displayDate(Month, Day, Year);
  displayTime(Hour, Minute, Second);

  if(Encoder_button == 0){
      settings_time = millis(); // if entering function due to button reset settings time
      settings_time_old = millis();
      settings = 1;
    }

  if(settings == 1){ // Encoder button was pressed. Enter setting adjustment mode
    settings_time = settings_time - settings_time_old; // read how long its been since an input
    Serial.print("  settings time; ");
    Serial.print(settings_time);
    if(settings_time > 5000){ // cut menu adjust out of loop if its been too long since input
      settings = 0;
      // also clear cursor here later
    }
    else{
      settings = 1;
    }
    menu_adjust();
    settings_time = millis();
  }

  Serial.println(); // print new line
}



void menu_adjust(){
  encoder rotation = nothing;
  int Encoder_button = digitalRead(SW);
  static bool select = 0; // flag for entering selection of specific menus
  if(select == 0){
    rotation = Encoder(); // returns if encoder rotated clockwise or counterclockwise
  }

  if(settings_time > 200 && Encoder_button == 0){
    select = 1;
  }
  else if(settings_time > 4900){
    select = 0; // allow a different menu item to be selected when leaving menu settings screen
  }

  if(rotation == clockwise){
    menu_location = menu_location+1;
    if(menu_location > 10){menu_location = 0;} // total of 11 menu locations
  }

  else if(rotation == counter_clockwise){
    menu_location = menu_location-1;
    if(menu_location < 0){menu_location = 10;}
  }

  Serial.print("  menu location=");
  Serial.print(menu_location);
  Serial.print("  select=");
  Serial.print(select);

  // adjust time in hours on the main clock
  if(menu_location == 0 && select == 1){
    int hour = encoder_count(24); // use encoder to select number between 1 and 12
    myRTC.setHour(hour); // adjust hour register in RTC
    int minute = myRTC.getMinute();
    int second = myRTC.getSecond();
    displayTime(hour, minute, second); // call display time function again to update display
  }


  // adjust time in minutes on main clock
  if(menu_location == 1 && select == 1){
    int minute = encoder_count(60); // use encoder to select number between 1 and 60
    myRTC.setMinute(minute); // adjust hour register in RTC
    int hour = myRTC.getHour(h12Flag, pmFlag);
    int second = myRTC.getSecond();
    displayTime(hour, minute, second); // call display time function again to update display
  }


  if(((int(millis()/500))%2==0) && (settings_time < 4900)){ //blink cursor every half second
    lcd.print("|");
  }
  else{
    lcd.print(" ");
  }

  // due to the display being strange the 4th row is an extension of the 2nd starting at 20
}
