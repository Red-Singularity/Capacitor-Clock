// Capacitor Clock main code
// Started 1/24/2026
// communicates  with a screen, an encoder, some buttons, a stepper motor, and some power circuitry
// to explode a small capacitor when the set time has been reached

#include <LiquidCrystal_I2C.h> // include I2C library
#include <AccelStepper.h> // using accel stepper library for motor positioning

#define MotorInterfaceType 4 // defines stepper as a 4 wire

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper myStepper(MotorInterfaceType, 1, 3, 2, 4);

LiquidCrystal_I2C lcd(0x27,20,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int stepsPerRevolution = 2048;

void setup() {
  // serial setup for debugging
  Serial.begin(115200);

  //stepper setup
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(50.0);
  myStepper.setSpeed(200);
  myStepper.moveTo(2048);

  // LCD setup
  lcd.begin(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Cap Clock V1");
  lcd.setCursor(0,1);
  lcd.print("By Mal & Dustin");
  delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly:

  displayFormat(); // display the main format tot he display
  displayDate(11, 6, 2000);



  /*
    if (myStepper.distanceToGo() == 0)
    myStepper.moveTo(-myStepper.currentPosition());

  // Move the motor one step
  myStepper.run();
  */

}
