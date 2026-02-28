
//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 2048;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 32, 25, 33, 26);

void setup() {
  // Nothing to do (Stepper Library sets pins as outputs)
}

void loop() {
  myStepper.setSpeed(10);
  myStepper.step(stepsPerRevolution);

}
