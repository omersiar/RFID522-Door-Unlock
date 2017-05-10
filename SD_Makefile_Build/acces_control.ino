/********************************************************************************************
 * RFID-based access control and time records storage into SD card                          *
 * Based on:    See requirements.txt                                                        *
 * Author:      Juan Reyes, Apr, 2017.                                                      *
 * Copyright:   Unlicense -> For more information, please refer to <http://unlicense.org>   *
 * Usage:       See readme.txt                                                              *
 ********************************************************************************************/

#include "pin_layout.h"
#include <RFID_module.h>
#include <RTC_module.h>
#include <SPI.h>
#include <LCD_module.h>
#include <Wire.h>

timeState time;                                // Create a time object acc. to a unique RTC_DS3231 HW
Adafruit_ST7735 tft (LCD_CS, LCD_DC, LCD_RST); // Instantiate an object using HW manufacturer's library
display display_1(&tft);                       // Create a display object acc to specific HW (this enables the usage of multiple displays)
MFRC522 rfid(RFID_SS, RFID_RST);               // Instantiate an object using HW manufacturer's library
rfid_unit rfid_unit_1(&rfid);
tag rfid_tag;                                  // Tag information structure (Key & assigned name)


void setup()
{
    time.initializeDS3231 ();                  // Setup for DS3231
    display_1.initializeTFT_LCD();             // Setup for LCD
    rfid_unit_1.initializeRFID();                          // Setup for RFID MFRC522 */
    display_1.initialize_SD_storage();         // Setup for records file in SD storage
    pinMode(ACC_DENIED, OUTPUT);
    pinMode(ACC_GRANTED, OUTPUT);

    // Enable this to adjust the RTC current time (ONLY ONCE)
    // rtc.setDateTime (DateTime (2017, 04, 23, 13, 05, 00, 1));
    // DateTime (Year,Month,Date,Hour, Minute, Second, Weekday) --> (Sunday = 1)
}


void loop()
{
    display_1.print_date_time(&time);            // LCD print current time & date
    time.update_time();                          // RTC Update current time
    display_1.access_default_status();           // Acces denied by default
    rfid_tag = rfid_unit_1.check_acces();        // Compare Tag info with authorized keys and parse user name

    if (rfid_unit_1.check_new_card()){                 // To avoid recurrent tag loading: Execute only if new card has been loaded
        display_1.validate_card(rfid_tag, &time);      // Validate and execute access control
        rfid_unit_1.clear_nuid();                      // Clear RFID NUIDs
    }
}
