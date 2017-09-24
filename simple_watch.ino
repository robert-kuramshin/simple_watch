#include <Wire.h>
#include "RTClib.h"
#include "LowPower.h"
#include <U8g2lib.h>

#define DS3231_I2C_ADDR             0x68
#define DS3231_TEMPERATURE_ADDR     0x11

U8G2_SH1106_128X64_NONAME_1_HW_I2C  u8g2(U8G2_R0);
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

String applications[] = {"clock","calendar"};

int last_second = 0;
int button_state = LOW;
/*
String makeGrid() {
  String grid[5][7];
  grid[0] = {"M","T","W","T","F","S","S"};
  int day_of_week = (now.dayOfTheWeek()+6)%7;
  int num_days = daysInMonth(now.month()-1);
  if(num_days==28 && now.year()%4==0) {
    num_days=29;
  }
  int starting_day = -(now.day()-day_of_week-7*int(now.day()/7))+1;

  for(int i = 0;i<num_days;i++) {
    int column = (i+starting_day)%7;
    int row = (i+starting_day)/7+1;
    grid[row][column] = i+1;
  }
  return grid;
}
*/
void buttonAction() {
  String t = ("ey lmao");
  u8g2.drawStr( 0, 15, t.c_str ());
}

void displayTime(DateTime now,int temp) {
  u8g2.setFont(u8g2_font_unifont_t_latin);
  String time[] = {String(now.hour()),String(now.minute()),String(now.second())};
  for (int i =0;i<sizeof(time);i++) {
    if(time[i].length()==1) {
      time[i]="0"+time[i];
    }
  }
  String t = (time[0] + ":" + time[1] + ":" + time[2]);
  
  u8g2.drawStr( 0, 15, t.c_str ());
  t = (String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()));
  u8g2.drawStr( 0, 35, t.c_str ());
  t = (String(daysOfTheWeek[now.dayOfTheWeek()]) + " " + String(temp)+"C");
  u8g2.drawStr( 0, 55, t.c_str ());
}

short getTemp()
{
  short temp;

  Wire.beginTransmission(DS3231_I2C_ADDR);
  Wire.write(DS3231_TEMPERATURE_ADDR);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_I2C_ADDR, 2);
  temp = Wire.read() << 8;
  temp |= Wire.read();
  return (temp / 256.);
}

void setup(void) {
  //pinMode(9, INPUT_PULLUP);
  u8g2.begin();
}

void loop(void) {
  //button_state = digitalRead(9);
  DateTime now = rtc.now();
  if (last_second != now.second()) {
    int temp = getTemp();
    u8g2.firstPage();
    last_second = now.second();
    do {
      displayTime(now,temp);
    } while ( u8g2.nextPage());
  }
  LowPower.powerDown(SLEEP_60MS, ADC_OFF, BOD_OFF);
  

}



