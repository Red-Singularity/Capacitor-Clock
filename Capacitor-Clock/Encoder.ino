encoder Encoder(){ // returns enumerated type encoder that is value clockwise or counter clockwise
  int newPos;
  encoder rotation; // create variable rotation as part of encoder enumerated type 

  static int pos = 0;
  rotary_encoder.tick();
  newPos = rotary_encoder.getPosition();

  if (pos != newPos) {
    settings_time = millis(); // reset settings menu timer
    settings_time_old = millis();

    if(pos < newPos){
      rotation = clockwise;
    }
    else if(pos > newPos){
      rotation = counter_clockwise;
    }
    pos = newPos;
  }

  else{
    rotation = nothing;
  }

  return rotation;
}

int encoder_count(int max){ // uses encoder to adjust between 0 and max number provided
  int newPos;
  static int counter = 5;
  static int pos = 0;
  rotary_encoder.tick();
  newPos = rotary_encoder.getPosition();

  if (pos != newPos) {
    settings_time = millis(); // reset settings menu timer
    settings_time_old = millis();

    Serial.print("counter; ");
    Serial.print(counter);

    if(pos > newPos){
      counter--;
      if(counter < 1){counter = max;}
    }
    else if(pos < newPos){
      counter++;
      if(counter > max){counter = 1;}
    }
    pos = newPos;
  }

  return counter;

}