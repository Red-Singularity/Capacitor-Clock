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
byte alarmDay; // 1-7 if day of week, 1-31 if date in month
byte alarmHour; // 1-12 if 12-hour mode, 0-23 if 24-hour mode
byte alarmMinute; // 00 - 59
byte alarmSecond; // 00 - 59
byte alarmBits = 0b00001000; // a bitfield used to determine frequency of alarm 0b00001000 is alarm 1 every day
bool alarmDy = true; // (true) if alarmDay is a day of the week, (false) if date in month
bool alarmH12Flag = true; // use 12 hour time mode for alarm
bool alarmPmFlag = true; // determines that 12 means noon


int menu_location = 0; // total different adjustable fields for clock display
int menu_location_old = 0;

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
  int Year = myRTC.getYear();
  int Month = myRTC.getMonth(CenturyBit);
  int Day = myRTC.getDate();
  int Hour = myRTC.getHour(h12Flag, pmFlag);
  int Minute = myRTC.getMinute();
  int Second = myRTC.getSecond();
  int Encoder_button = digitalRead(SW);

  bool Alarm_Button = digitalRead(Button_1);

  if(Alarm_Button == 1){
    alarm_functions();
  }

  displayTime();
  displayAlarm();
  displayDate(Month, Day, Year);

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
    if(menu_location > 8){menu_location = 0;} // total of 11 menu locations
  }

  else if(rotation == counter_clockwise){
    menu_location = menu_location-1;
    if(menu_location < 0){menu_location = 8;}
  }

  //Serial.print("  menu location=");
  //Serial.print(menu_location);
  //Serial.print("  select=");
  //Serial.print(select);

  // adjust time in hours on the main clock
  if(menu_location == 0 && select == 1){
    int hour = encoder_count(24); // use encoder to select number between 1 and 24
    myRTC.setHour(hour); // adjust hour register in RTC
    displayTime(); // call display time function again to update display
  }


  // adjust time in minutes on main clock
  else if(menu_location == 1 && select == 1){
    int minute = encoder_count(60); // use encoder to select number between 1 and 60
    myRTC.setMinute(minute); // adjust hour register in RTC
    myRTC.setSecond(0); // when adjusting time in minutes set seconds to 0
    displayTime(); // call display time function again to update display
  }

  //adjust alarm in hours
  else if(menu_location == 2 && select == 1){
    byte A1Day, A1Hour, A1Minute, A1Second, AlarmBits;
    bool A1Dy, A1h12, A1PM;
    myRTC.getA1Time(A1Day, A1Hour, A1Minute, A1Second, AlarmBits, A1Dy, A1h12, A1PM);  // get all alarm 1 values
    A1Hour = encoder_count(24); // use encoder to select number between 1 and 12
    //Serial.print("  A1hour=");
    //Serial.print(A1Hour);
    if(A1Hour > 12){
      A1Hour = A1Hour-12; // bring back down to 12 hour format
      A1PM = true; //set PM flag
    }
    else{
      A1PM= false;
    }
    myRTC.setA1Time(A1Day, A1Hour, A1Minute, A1Second, AlarmBits, A1Dy, A1h12, A1PM); // set all alarm 1 values

    displayAlarm();
  }

  else if(menu_location == 3 && select == 1){
    byte A1Day, A1Hour, A1Minute, A1Second, AlarmBits;
    bool A1Dy, A1h12, A1PM;
    myRTC.getA1Time(A1Day, A1Hour, A1Minute, A1Second, AlarmBits, A1Dy, A1h12, A1PM);  // get all alarm 1 values
    A1Minute = encoder_count(60); // use encoder to select number between 1 and 60
    myRTC.setA1Time(A1Day, A1Hour, A1Minute, A1Second, AlarmBits, A1Dy, A1h12, A1PM); // set all alarm 1 values
    displayAlarm();
  }

  //adjust alarm in minutes
  else if(menu_location == 3 && select == 1){

  }

  //adjust month
  else if(menu_location == 6 && select == 1){
    int month = encoder_count(12);
    myRTC.setMonth(month);
    int day = myRTC.getDate();
    int year = myRTC.getYear();
    displayDate(month, day, year);
  }

  //adjust day
  else if(menu_location == 7 && select == 1){
    bool CenturyBit; // this bit is required as an input to the month function and cannot be input as a true or false to the function directly
    int month = myRTC.getMonth(CenturyBit);
    int day = encoder_count(31);
    myRTC.setDate(day);
    int year = myRTC.getYear();
    displayDate(month, day, year);
  }

  //adjust year
  else if(menu_location == 8 && select == 1){
    bool CenturyBit; // this bit is required as an input to the month function and cannot be input as a true or false to the function directly
    int month = myRTC.getMonth(CenturyBit);
    int day = myRTC.getDate();
    int year = encoder_count(99);
    myRTC.setYear(year);
    displayDate(month, day, year);
  }







  //display on screen what setting is being adjusted by row
  Serial.print("  menu_location = ");
  Serial.print(menu_location);
  Serial.print("  menu_location old=");
  Serial.print(menu_location_old);

  if(menu_location < 2){
    if(menu_location != menu_location_old){
      lcd.setCursor(19,1); // ensure previous menu lcoations are erased if on different row
      lcd.print(" ");
      lcd.setCursor(19,2);
      lcd.print(" ");
      lcd.setCursor(39,1);
      lcd.print(" ");
      lcd.setCursor(19,0); // // set new location to display menu location
    }

    lcd.setCursor(19,0); // set new location to display menu location
  }

  else if(menu_location < 4){
    if(menu_location != menu_location_old){
      lcd.setCursor(19,0); // ensure previous menu lcoations are erased if on different row
      lcd.print(" ");
      lcd.setCursor(19,2);
      lcd.print(" ");
      lcd.setCursor(39,1);
      lcd.print(" ");
    }

    lcd.setCursor(19,1); // set new location to display menu location
  }

  else if(menu_location < 6){
    if(menu_location != menu_location_old){
      lcd.setCursor(19,0); // ensure previous menu lcoations are erased if on different row
      lcd.print(" ");
      lcd.setCursor(19,1);
      lcd.print(" ");
      lcd.setCursor(39,1);
      lcd.print(" ");
    }

    lcd.setCursor(19,2); // set new location to display menu location
  }

  else{
    if(menu_location != menu_location_old){
      lcd.setCursor(19,0); // ensure previous menu lcoations are erased if on different row
      lcd.print(" ");
      lcd.setCursor(19,1);
      lcd.print(" ");
      lcd.setCursor(19,2);
      lcd.print(" ");
    }

    lcd.setCursor(39,1); // set new location to display menu location
  }

  menu_location_old = menu_location; // update menu location old

  if(settings_time < 4900){ //blink cursor every half second // add this to function to blink (int(millis()/500))%2==0) && 
    lcd.print(menu_location);
  }
  else{
    lcd.print(" "); // always leave space blank before menu adjust function stops being called
  }

}

void alarm_functions(){
  static bool alarm_status;
  bool alarm_button; // alarm button state
  static int debounce_time;
  //byte Alarm = 

  alarm_button = digitalRead(Button_1);

  alarm_status = myRTC.checkAlarmEnabled(1); // check alarm 1 status;

  // enable alarm if not already enabled

  // disable alarm if enabled


}
