#ifndef _RTC_DS3231_
#define _RTC_DS3231_

#include <SPI.h>
#include <Wire.h>
#include <Sodaq_DS3231.h>  // Include required libraries for RTCDS3231

////////////////////////////////////////////////////////////////////////

class timeState {
public:
    void initializeDS3231 (); //Initialize wire transfer and instansiate a RTC DS3231
    void update_time();
    inline DateTime get_current_time()   {return current;}
    inline String get_week_day()         {return weekDay[current.dayOfWeek()-1];}
    inline bool get_u_date()             {return u_date;}
    inline bool get_u_hour()             {return u_hour;}
    inline bool get_u_minute()           {return u_minute;}
    inline bool get_u_second()           {return u_second;}

protected:
    DateTime current, prev;                  // Time state variables
    char weekDay[8][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; // Define the names of week day for diff languages.
    bool u_year, u_month, u_date, u_hour, u_minute, u_second; // Return true if the value has been updated
};


#endif // _RTC_DS3231_
