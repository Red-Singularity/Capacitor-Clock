
// main code pertaining to the LCD display. shows date and time

void displayFormat(){
  lcd.setCursor(0,0);
  lcd.print("Time:    :  :      ");
  lcd.setCursor(0,1);
  lcd.print("Alarm:   :  :      ");
  lcd.setCursor(0,2);
  lcd.print("Left:    Spent:   ");
  lcd.setCursor(20,1);
  lcd.print("Date:   /  /    ");
}

void displayTime(int hour, int minute, int second){
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


  if(hour != minuteOld){ // check to see if value updated to only update display when necessarry
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

}

void displayDate(int month, int day, int year){
  String month_s = String(month); // cast integers to strings
  String day_s = String(day);
  String year_s = String(year);

  // due to the display being strange the 4th row is an extension of the 2nd starting at 20
  lcd.setCursor(26,1);
  lcd.print(month_s);
  lcd.setCursor(29,1);
  lcd.print(day_s);
  lcd.setCursor(32,1);
  lcd.print(year_s);
}
