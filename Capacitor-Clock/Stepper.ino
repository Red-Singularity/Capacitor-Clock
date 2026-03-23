void stepperMove(){
  float current; // measured current
  float shuntVoltage;
  float shunt = .1; // shunt is 100m ohms
  int speed = 1; // speed in RPM

  int steps = 10; // steps per time this function is called

  shuntVoltage = analogRead(I_sense); // read I_sense pin
  shuntVoltage = (shuntVoltage/4096)*3.3; // turn analog voltage reading into voltage reading
  current = shuntVoltage / shunt; //total shunt current read

  Serial.print("  Current= ");
  Serial.println(current);

  if(alarmMain == 1){ // if alarm is going off move stepper
    if(current > 1){ // if current reading is greater than 1 amp stop rotating
      myStepper.setSpeed(0);
    }
    else{
      myStepper.setSpeed(speed); // rotate
      myStepper.step(steps);
    }
  }
}