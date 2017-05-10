// Sodaq_DS3231 Class is a modified version of DS3231.
// DS3231 Class is by Seeed Technology Inc(http://www.seeedstudio.com) and used
// in Seeeduino Stalker v2.1 for battery management(MCU power saving mode)
// & to generate timestamp for data logging. DateTime Class is a modified
// version supporting day-of-week.

// Original DateTime Class and its utility code is by Jean-Claude Wippler at JeeLabs
// http://jeelabs.net/projects/cafe/wiki/RTClib 
// Released under MIT License http://opensource.org/licenses/mit-license.php

#ifndef SODAQ_DS3231_H
#define SODAQ_DS3231_H

#include <Arduino.h>
#include <stdint.h>




// Simple general-purpose date/time class (no TZ / DST / leap second handling!)
class DateTime {
public:
    DateTime (long t =0);
    DateTime (uint16_t year, uint8_t month, uint8_t date,
              uint8_t hour, uint8_t min, uint8_t sec, uint8_t wday);
    DateTime (const char* date, const char* time);

    uint8_t second() const      { return ss; }
    uint8_t minute() const      { return mm; } 
    uint8_t hour() const        { return hh; }
 
    uint8_t date() const        { return d; }
    uint8_t month() const       { return m; }
    uint16_t year() const       { return 2000 + yOff; }		// Notice the 2000 !

    uint8_t dayOfWeek() const   { return wday;}  /*Su=1 Mo=2 Tu=3 We=4 Th=5 Fr=6 Sa=7 */

    // 32-bit time as seconds since 1/1/2000
    uint32_t get() const;
    // 32-bit number of seconds since Unix epoch (1970-01-01)
    uint32_t getEpoch() const;

    void addToString(String & str) const;

protected:
    uint8_t yOff, m, d, hh, mm, ss, wday;
};

// These are the constants for periodicity of enableInterrupts() below.
#define EverySecond     0x01
#define EveryMinute     0x02
#define EveryHour       0x03

// RTC DS3231 chip connected via I2C and uses the Wire library.
// Only 24 Hour time format is supported in this implementation
class Sodaq_DS3231 {
public:
    uint8_t begin(void);

    void setDateTime(const DateTime& dt);  //Changes the date-time
    void setEpoch(uint32_t ts); // Set the RTC using timestamp (seconds since epoch)
    DateTime now();            //Gets the current date-time

    DateTime makeDateTime(unsigned long t);

    //Decides the /INT pin's output setting
    //periodicity can be any of following defines: EverySecond, EveryMinute, EveryHour 
    void enableInterrupts(uint8_t periodicity);
    void enableInterrupts(uint8_t hh24, uint8_t mm,uint8_t ss);
    void disableInterrupts();
    void clearINTStatus();

    void convertTemperature();
    float getTemperature();
private:
    uint8_t readRegister(uint8_t regaddress);
    void writeRegister(uint8_t regaddress, uint8_t value);
};

extern Sodaq_DS3231 rtc;

#endif
