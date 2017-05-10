#include "RTC_module.h"

DateTime current, prev;
bool u_date, u_month, u_year, u_hour, u_minute, u_second;

void timeState::initializeDS3231 () //Initialize wire transfer and instansiate a RTC DS3231
{
  Wire.begin();
  rtc.begin();
}

void timeState::update_time()
{
    current = rtc.now();          // Get the current date-time
    if (current.year() == prev.year())      {u_year = false;}    else{u_year = true;}
    if (current.month() == prev.month())    {u_month = false;}   else{u_month = true;}
    if (current.date() == prev.date())      {u_date = false;}    else{u_date = true;}
    if (current.hour() == prev.hour())      {u_hour = false;}    else{u_hour = true;}
    if (current.minute() == prev.minute())  {u_minute = false;}  else{u_minute = true;}
    if (current.second() == prev.second())  {u_second = false;}  else {u_second = true;}
    prev = current;
}
