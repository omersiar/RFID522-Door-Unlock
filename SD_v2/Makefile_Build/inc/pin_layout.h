#ifndef _PIN_LAYOUT_
#define _PIN_LAYOUT_

/*Fixed pins for Arduino Mega 2560:
   * SDA           20
   * SCL           21
   * SPI MOSI      51
   * SPI MISO      50
   * SPI SCK       52
*/


/* Connections from RTC DS3231 to Arduino Mega 2560:
   * VCC           5V
   * GND
   * SDA           20
   * SCL           21
*/


/* Connections from SainSmart 1.8 ST7735R TFT LCD Module with MicroSD to Arduino Mega 2560:*/
#define LCD_CS      46  // SainSmart: CS
#define LCD_DC      47  // SainSmart: RS/DC
#define LCD_RST     48  // SainSmart: RES
#define SD_CS       49  // SainSmart: SD card CS pin
// SD_MISO          50  // SainSmart: SD card MISO pin
// SDA &  SD_MOSI   51  // Parallel connection
// SCL & SD_SCLK    52  // Parallel connection


/* Connection from MFRC522 to Arduino Mega 2560:*/
// VCC              3.3V
#define RFID_RST    44  // MFRC522 Connection pin (RST)
#define RFID_SS     45  // MFRC522 Connection pin (SDA)
// MISO             50  // Parallel connection
// MOSI             51  // Parallel connection
// SCK              52  // Parallel connection


/* Output Connections */

#define ACC_DENIED  42  // Access denied
#define ACC_GRANTED 43  // Access granted



#endif // _PIN_LAYOUT_
