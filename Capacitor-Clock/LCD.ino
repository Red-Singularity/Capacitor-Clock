
// main code pertaining to the LCD display. shows date and time

void displayFormat(){
  lcd.setCursor(0,0);
  lcd.print("Time:    :  :      ");
  lcd.setCursor(0,1);
  lcd.print("Alarm:   :      ");
  lcd.setCursor(0,2);
  lcd.print("Left:    Spent:   ");
  lcd.setCursor(20,1);
  lcd.print("Date:   /  /    ");
}

void displayFormatAlarm(){
  lcd.setCursor(0,1);
  lcd.print("Alarm:   :      ");
}

void displayTime(){
  int hour = myRTC.getHour(h12Flag, pmFlag);
  int minute = myRTC.getMinute();
  int second = myRTC.getSecond();

  static int hourOld;
  static int minuteOld;
  static int secondOld;
  String hour_s = String(hour); // cast integers to strings
  String minute_s = String(minute);
  String second_s = String(second);

  if(hour != hourOld){ // check to see if value updated to only update display when necessarry
  hourOld = hour;
    if(hour>9){ // if value is greater than 9 the formatting doesnt need adjustment
      lcd.setCursor(7,0);
      lcd.print(hour_s);
    }
    else{ // sets formating properly 
      lcd.setCursor(7,0);
      lcd.print("0"); // puts leading 0
      lcd.setCursor(8,0);
      lcd.print(hour_s);
    }
  }


  if(minute != minuteOld){ // check to see if value updated to only update display when necessarry
  minuteOld = minute;
    if(minute>9){ // if value is greater than 9 the formatting doesnt need adjustment
      lcd.setCursor(10,0);
      lcd.print(minute_s);
    }
    else{ // sets formating properly 
      lcd.setCursor(10,0);
      lcd.print("0"); // puts leading 0
      lcd.setCursor(11,0);
      lcd.print(minute_s);
    }
  }


  if(second != secondOld){ // check to see if value updated to only update display when necessarry
  secondOld = second;
    if(second>9){ // if value is greater than 9 the formatting doesnt need adjustment
      lcd.setCursor(13,0);
      lcd.print(second_s);
    }
    else{ // sets formating properly 
      lcd.setCursor(13,0);
      lcd.print("0"); // puts leading 0
      lcd.setCursor(14,0);
      lcd.print(second_s);
    }
  }

  lcd.setCursor(16,0);

  if(pmFlag == true){
    lcd.print("PM");
  }
  else{
    lcd.print("AM");
  }

}

void displayAlarm(){
  //* Retrieves values from the Alarm 1 time registers then stores the values
  //* in the external variables passed to the parameter list.
  //* NOTE: previous values of the parameter variables are over-written.
  byte A1Day, A1Hour, A1Minute, A1Second;
  bool A1Dy, A1h12, A1PM;
  bool alarm_status;
  myRTC.getA1Time(A1Day, A1Hour, A1Minute, A1Second, alarmBits, A1Dy, A1h12, A1PM);


  //int hour = A1Hour;
  //int minute = A1Minute;

  static byte hourOld;
  static byte minuteOld;
  String hour_s = String(A1Hour); // cast integers to strings
  String minute_s = String(A1Minute);

  if(alarmMain == 0){ // display normal alarm time if alarm isnt actively going off
    if(A1Hour != hourOld){ // check to see if value updated to only update display when necessarry
    hourOld = A1Hour;
      if(A1Hour>9){ // if value is greater than 9 the formatting doesnt need adjustment
        lcd.setCursor(7,1);
        lcd.print(hour_s);
      }
      else{ // sets formating properly 
        lcd.setCursor(7,1);
        lcd.print("0"); // puts leading 0
        lcd.setCursor(8,1);
        lcd.print(hour_s);
      }
    }


    if(A1Minute != minuteOld){ // check to see if value updated to only update display when necessarry
    minuteOld = A1Minute;
      if(A1Minute>9){ // if value is greater than 9 the formatting doesnt need adjustment
        lcd.setCursor(10,1);
        lcd.print(minute_s);
      }
      else{ // sets formating properly 
        lcd.setCursor(10,1);
        lcd.print("0"); // puts leading 0
        lcd.setCursor(11,1);
        lcd.print(minute_s);
      }
    }

    lcd.setCursor(13,1);
    if(A1PM == true){
      lcd.print("PM");
    }
    else{
      lcd.print("AM");
    }
  }

  else{
    lcd.setCursor(7,1);
    lcd.print("WAKE UP!");
    hourOld = 0;
    minuteOld = 0;
  }

  lcd.setCursor(16,1);
  alarm_status = myRTC.checkAlarmEnabled(1); // check alarm 1 status;
  if(alarm_status == 0){
    lcd.print("  ");
  }
  else{
    lcd.print("EN");
  }
}

void displayDate(int month, int day, int year){
  static int monthOld;
  static int dayOld;
  static int yearOld;

  year = year+2000; // add 2000 to years since only tens and 1s are captured

  String month_s = String(month); // cast integers to strings
  String day_s = String(day);
  String year_s = String(year);

  // due to the display being strange the 4th row is an extension of the 2nd starting at 20

  if(month != monthOld){ // check to see if value updated to only update display when necessarry
  monthOld = month;
    if(month>9){ // if value is greater than 9 the formatting doesnt need adjustment
      lcd.setCursor(26,1);
      lcd.print(month_s);
    }
    else{ // sets formating properly 
      lcd.setCursor(26,1);
      lcd.print(" "); // puts leading space
      lcd.print(month_s);
    }
  }

  if(day != dayOld){ // check to see if value updated to only update display when necessarry
  dayOld = day;
    if(day>9){ // if value is greater than 9 the formatting doesnt need adjustment
      lcd.setCursor(29,1);
      lcd.print(day_s);
    }
    else{ // sets formating properly 
      lcd.setCursor(30,1);
      lcd.print(day_s);
      lcd.setCursor(29,1);
      lcd.print(" ");
    }
  }

  // year is a number between 0 and 99
  if(year != yearOld){ // check to see if value updated to only update display when necessarry
  yearOld = year;
    if(year>9){ // if value is greater than 9 the formatting doesnt need adjustment
      lcd.setCursor(32,1);
      lcd.print(year_s);
    }
    else{ // sets formating properly 
      lcd.setCursor(32,1);
      lcd.print("0"); // puts leading space
      lcd.print(year_s);
    }
  }

}
