
// main code pertaining to the LCD display. shows date and time

void displayFormat(){
  lcd.setCursor(0,0);
  lcd.print("Time:   :  :  ");
  lcd.setCursor(0,1);
  lcd.print("Alarm:   :  :  ");
  lcd.setCursor(0,1);
  lcd.print("Date:   /  /    ");
  )
}

void displayTime(int hours, int minutes, int days){
  lcd.setCursor(7,0);
  lcd.print(hours);
  lcd.setCursor(10,0);
  lcd.print(minutes);
  lcd.setCursor(13,0);
}

void displayAlarm(){

}

void displayDate(int month, int day, int year){
  String month_s = String(month); // cast integers to strings
  String day_s = String(day);
  String year_s = String(year);

  lcd.setCursor(6,1);
  lcd.print(month_s);
  lcd.setCursor(9,1);
  lcd.print(day_s);
  lcd.setCursor(12,1);
  lcd.print(year_s);
}
