//Interrupts for Battery management/saving using MCU power down mode. /INT from DS3231 is connected to INT0 of MCU.

#include <avr/sleep.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"

static uint8_t prevSecond=0; 

void setup () 
{
     /*Initialize INT0 for accepting interrupts */
     PORTD |= 0x04; 
     DDRD &=~ 0x04;
  
     Serial.begin(57600);
     Wire.begin();
    
     rtc.begin();
     attachInterrupt(0, INT0_ISR, FALLING); 
     
     //Enable Interrupt 
     rtc.enableInterrupts(EveryMinute); //interrupt at  EverySecond, EveryMinute, EveryHour
     // or this
     //rtc.enableInterrupts(18,4,0);    // interrupt at (h,m,s)
}


void loop () 
{

    DateTime now = rtc.now(); //get the current date-time    
    if((now.second()) !=  prevSecond )
    {
    //print only when there is a change in seconds
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.date(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println(' ');
    }
    prevSecond = now.second();
    rtc.clearINTStatus();
 
} 

  
//Interrupt service routine for external interrupt on INT0 pin conntected to /INT
void INT0_ISR()
{
  //Keep this as short as possible. Possibly avoid using function calls
  
   Serial.println(" External Interrupt detected ");
}
