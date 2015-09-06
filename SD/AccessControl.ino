/*
  Arduino RFID Access Control

  Security !

  To keep it simple we are going to use Tag's Unique IDs
  as only method of Authenticity. It's simple and not hacker proof.
  If you need security, don't use it unless you modify the code

  Copyright (C) 2015 Omer Siar Baysal

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include <SPI.h>          // RC522 Module uses SPI protocol
#include <MFRC522.h>	    // Library for Mifare RC522 Devices
#include <SD.h>           // We are going to read and write PICC's UIDs from/to SD

#include "Pinning.h"      // Include our Pinnings
#include "Settings.h"     // Include our variable Settings

/*
	Maybe you want to use a servo on the output pin
	Servos can lock and unlock door locks too
	There are examples out there.
*/

// #include <Servo.h>   or
// #include <SoftwareServo.h>

boolean match = false;           // initialize card match to false
boolean programMode = false;     // initialize programming mode to false

int successRead;		             // Variable integer to keep if we have Successful Read from Reader

byte readCard[4];		             // Stores scanned ID read from RFID Module
byte masterCard[4];		           // Stores master card's ID

char username[8] = {'\0'};
char filename[] = "XXXXXXXXXXXXXXX.DAT";  // Stores variable filename
char extension[] = "DAT";                 // sometimes the extension gets modified
char dir[] = "/PICCS/";

MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);	        // Create MFRC522 instance.

///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {
  //Arduino Pin Configuration
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(OUTPUT_PIN, OUTPUT);            // Be careful how output circuit behave on while resetting or power-cycling your Arduino
  digitalWrite(OUTPUT_PIN, OUTPUT_OFF);		// Make sure output is off
  digitalWrite(LED_RED, LED_OFF);	        // Make sure led is off
  digitalWrite(LED_GREEN, LED_OFF);	      // Make sure green is off
  digitalWrite(LED_BLUE, LED_OFF);        // Make sure blue is off

  //Initialize
  Serial.begin(57600);	                                // Initialize serial communications with PC
  Serial.println(F("Access Control v4.6_SD"));        // For debugging purposes
  SPI.begin();                                        // MFRC522 and SD Card Hardware uses SPI protocol
  if (!SD.begin(SD_SS_PIN)) {	                        // Initialize SD Hardware
    Serial.println(F("SD initialization failed!"));   // Could not initialize
    redSolid();
    while (true);                                     // Do not go further
  }
  Serial.println(F("SD initialization done."));       // Yay all SPI slaves work
  mfrc522.PCD_Init();                                 // Initialize MFRC522 Hardware
  ShowReaderDetails();	 // Show details of PCD - MFRC522 Card Reader details

  //If you set Antenna Gain to Max it will increase reading distance
  //mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);

  checkMaster();      // Check if masterCard defined
  Serial.println(F("Everything Ready"));
  Serial.println(F("Waiting PICCs to be scanned"));
  cycleLeds();        // Lets give user some feedback that hardware initialized by cycling leds
}



///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop () {
  do {
    successRead = getID(); 	// sets successRead to 1 when we get read from reader otherwise 0
    if (programMode) {
      cycleLeds();              // Program Mode cycles through RGB waiting to read a new card
    }
    else {
      blueSolid(); 		// Normal mode, blue Power LED is on, all others are off
    }
  }
  while (!successRead); 	// the program will not go further while you not get a successful read
  if (programMode) {
    // Print the control menu:
    printMenu();
    // Then wait for any serial data to come in:
    while (!Serial.available());
    // Once serial data is received, call parseMenu to act on it:
    parseMenu(Serial.read());
  }
  if (isMaster(readCard)) {  	// If scanned card's ID matches Master Card's ID enter program mode
    Serial.println(F("Hello Master - Entered Program Mode"));
    programMode = true;
  }
  else {
    if (findID()) {	// If not, check if we can find it
      Serial.println(F("Welcome, You shall pass"));
      granted(300);        	// Open the door lock for 300 ms
    }
    else {			// If not, show that the ID was not valid
      Serial.println(F("You shall not pass"));
      denied();
    }
  }
}

void getFilename() {  // We will store UIDs as files on SD card
  sprintf(filename, "%s%02x%02x%02x%02x.%s", dir, readCard[0], readCard[1], // Convert readCard data to char and append extension
          readCard[2], readCard[3], extension);
}

boolean findID () {  // Check If we can find UID's specific file
  File fileopen = SD.open(filename);
  if (fileopen) {
    fileopen.close();  // Found it close
    return true;
  }
  else {  	// If not, return false
  }
  return false;
}

void writeID (char* user) {
  File filewrite = SD.open(filename, FILE_WRITE);
  filewrite.print(user);
  filewrite.close();
  if (SD.exists(filename)) {
    Serial.println(F("Succesfully added ID record"));
    greenBlink(3);
  }
  else {
    Serial.println(F("Failed to add record"));
    redBlink(3);
  }
}

void removeID () {
  SD.remove(filename);
  if (SD.exists(filename)) {
    Serial.println(F("Failed to remove. Record still exists"));
    redBlink(3);
  }
  else {
    Serial.println(F("Succesfully removed ID record"));
    blueBlink(3);
  }
}

/////////////////////////////////////////  Access Granted    ///////////////////////////////////
void granted (int setDelay) {
  digitalWrite(OUTPUT_PIN, LOW); 		// Unlock door!
  delay(setDelay); 			// Hold door lock open for given seconds
  digitalWrite(OUTPUT_PIN, HIGH); 		// Relock door
  greenSolid();
}

///////////////////////////////////////// Access Denied  ///////////////////////////////////
void denied() {
  redSolid();
}


///////////////////////////////////////// Check Bytes   ///////////////////////////////////
boolean checkTwo ( byte a[], byte b[] ) {
  if ( a[0] != NULL ) 			// Make sure there is something in the array first
    match = true; 			// Assume they match at first
  for ( int k = 0; k < 4; k++ ) { 	// Loop 4 times
    if ( a[k] != b[k] ) 		// IF a != b then set match = false, one fails, all fail
      match = false;
  }
  if ( match ) { 			// Check to see if if match is still true
    return true; 			// Return true
  }
  else  {
    return false; 			// Return false
  }
}

////////////////////// Check readCard IF is masterCard   ///////////////////////////////////
// Check to see if the ID passed is the master programing card
boolean isMaster( byte test[] ) {
  if ( checkTwo( test, masterCard ) )
    return true;
  else
    return false;
}
