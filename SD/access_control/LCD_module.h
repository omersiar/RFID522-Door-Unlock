#ifndef _TFT_LCD_
#define _TFT_LCD_

#include "pin_layout.h"
#include "RFID_module.h"
#include "RTC_module.h"
#include <SD.h>
#include <Adafruit_GFX.h>        // Core graphics library for TFT LCD - GREENTAB*!
#include <Adafruit_ST7735.h>     // Hardware-specific library for TFT LCD - GREENTAB*!
                                 //GREENTAB*: Specific implementation for GREENTAB_LCD_HW, refer to Adafruit library documentation.
#include <Sodaq_DS3231.h>  // Include required libraries for RTCDS3231

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

////////////////////////////////////////////////////////////////////////

class display {
public:
    display(Adafruit_ST7735* lcd);              // Constructor for specific Adafruit Hardware
    // display(ANY HARDWARE* lcd);              // Can be defined for any other specific HW. (REUSE!)
    void initializeTFT_LCD();                   // Setup for TFT LCD - GREENTAB*!
    void initialize_SD_storage();
    void write_time_register(String name, timeState* myTime);
    void access_default_status();
    void print_date_time(timeState* myTime);
    bool validate_card(tag my_tag, timeState* my_time);

private:
    Adafruit_ST7735* disp_pntr;
    File csv_file;
};


#endif // _TFT_LCD_
