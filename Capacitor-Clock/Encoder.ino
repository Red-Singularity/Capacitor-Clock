void Encoder(){
  static int counter = 0; 
  static int aState;
  static int aLastState;  

  aState = digitalRead(CLK); // Reads the "current" state of the CLK
  // If the previous and the current state of the CLK are different, that means a Pulse has occured
  if (aState != aLastState){     
    // If the DT state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(DT) != aState) { 
      counter ++;
    } else {
      counter --;
    }
    Serial.print("Position: ");
    Serial.println(counter);
  } 
  aLastState = aState; // Updates the previous state of the outputA with the current state

}