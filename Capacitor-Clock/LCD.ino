
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
  String hour_s = String(hour); // cast integers to strings
  String minute_s = String(minute);
  String second_s = String(second);

  lcd.setCursor(7,0);
  lcd.print(hour_s);
  lcd.setCursor(10,0);
  lcd.print(minute_s);
  lcd.setCursor(13,0); 
  lcd.print(second_s);

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
