/* Arduino RC522 RFID Door Unlocker
 * July/2014 Omer Siar Baysal
 * 
 * Unlocks a Door (controls a relay actually)
 * using a RC522 RFID reader with SPI interface on your Arduino
 * You define a Master Card which is act as Programmer
 * then you can able to choose card holders who able to unlock
 * the door or not.
 * 
 * Easy User Interface
 *
 * Just one RFID tag needed whether Delete or Add Tags
 * You can choose to use Leds for output or
 * Serial LCD module to inform users. Or you can use both
 *
 * Stores Information on EEPROM
 *
 * Information stored on non volatile Arduino's EEPROM 
 * memory to preserve Users' tag and Master Card
 * No Information lost if power lost. 
 * EEPROM has unlimited Read cycle but 100,000 limited Write cycle. 
 * 
 * Security
 * 
 * To keep it simple we are going to use Tag's Unique IDs
 * It's simple, a bit secure, but not hacker proof.
 *
 * MFRC522 Library also lets us to use some authentication
 * mechanism, writing blocks and reading back
 * and there is great example piece of code
 * about reading and writing PICCs
 * here > http://makecourse.weebly.com/week10segment1.html
 *
 * If you rely on heavy security, figure it out how RFID system
 * can be secure yourself (personally very curious about it)
 * 
 * Credits
 *
 * Omer Siar Baysal who put together this project
 *
 * Idea and most of code from Brett Martin's project
 * http://www.instructables.com/id/Arduino-RFID-Door-Lock/
 * www.pcmofo.com
 *
 * MFRC522 Library
 * https://github.com/miguelbalboa/rfid
 * Based on code Dr.Leong   ( WWW.B2CQSHOP.COM )
 * Created by Miguel Balboa (circuitito.com), Jan, 2012.
 * Rewritten by Søren Thing Andersen (access.thing.dk), fall of 2013 
 * (Translation to English, refactored, comments, anti collision, cascade levels.)
 *
 * Arduino Forum Member luisilva for His Massive Code Correction
 * http://forum.arduino.cc/index.php?topic=257036.0
 * http://forum.arduino.cc/index.php?action=profile;u=198897
 *
 * License
 *
 * You are FREE what to do with this code
 * Just give credits who put effort on this code
 *
 * "PICC" short for Proximity Integrated Circuit Card (RFID Tags)
 */

#include <EEPROM.h>  // We are going to read and write PICC's UIDs from/to EEPROM
#include <SPI.h>      // RC522 Module uses SPI protocol
#include <MFRC522.h>   // Library for Mifare RC522 Devices

/* Instead of a Relay maybe you want to use a servo
 * Servos can lock and unlock door locks too
 * There are examples out there.
 */

// #include <Servo.h>

/* For visualizing whats going on hardware
 * we need some leds and
 * to control door lock a relay and a wipe button
 * (or some other hardware)
 * Used common anode led,digitalWriting HIGH turns OFF led
 * Mind that if you are going to use common cathode led or
 * just seperate leds, simply comment out #define COMMON_ANODE,
 */

#define COMMON_ANODE

#ifdef COMMON_ANODE
#define LED_ON LOW
#define LED_OFF HIGH
#else
#define LED_ON HIGH
#define LED_OFF LOW
#endif

#define redLed 7
#define greenLed 6
#define blueLed 5
#define relay 4
#define wipeB 3 // Button pin for WipeMode

boolean match = false; // initialize card match to false
boolean programMode = false; // initialize programming mode to false

int successRead; // Variable integer to keep if we have Successful Read from Reader

byte storedCard[4];   // Stores an ID read from EEPROM
byte readCard[4];           // Stores scanned ID read from RFID Module
byte masterCard[4]; // Stores master card's ID read from EEPROM

/* We need to define MFRC522's pins and create instance
 * Pin layout should be as follows (on Arduino Uno):
 * MOSI: Pin 11 / ICSP-4
 * MISO: Pin 12 / ICSP-1
 * SCK : Pin 13 / ICSP-3
 * SS : Pin 10 (Configurable)
 * RST : Pin 9 (Configurable)
 * look MFRC522 Library for
 * pin configuration for other Arduinos.
 */

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {
  //Arduino Pin Configuration
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH); // Make sure door is locked
  digitalWrite(redLed, LED_OFF); // Make sure led is off
  digitalWrite(greenLed, LED_OFF); // Make sure led is off
  digitalWrite(blueLed, LED_OFF); // Make sure led is off
  
  //Protocol Configuration
  Serial.begin(9600);	 // Initialize serial communications with PC
  SPI.begin();           // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();    // Initialize MFRC522 Hardware
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max); //Set Antenna Gain to Max- this will increase reading distance

  //Wipe Code if Button Pressed while setup run (powered on) it wipes EEPROM
  pinMode(wipeB, INPUT_PULLUP);  // Enable pin's pull up resistor
  if (digitalRead(wipeB) == LOW) {     // when button pressed pin should get low, button connected to ground
    digitalWrite(redLed, LED_ON);   // Red Led stays on to inform user we are going to wipe
    Serial.println("Wipe Button Pressed");
    Serial.println("You have 5 seconds to Cancel");
    Serial.println("This will be remove all records and cannot be undone");
    delay(5000);    // Give user enough time to cancel operation
    if (digitalRead(wipeB) == LOW) {  // If button still be pressed, wipe EEPROM
      Serial.println("Starting Wiping EEPROM");
      for (int x=0; x<1024; x=x+1){ //Loop end of EEPROM address
        if (EEPROM.read(x) == 0){ //If EEPROM address 0 
          // do nothing, already clear, go to the next address in order to save time and reduce writes to EEPROM
        } 
        else{
          EEPROM.write(x, 0); // if not write 0, it takes 3.3mS
        }
      }
      Serial.println("Wiped");
      digitalWrite(redLed, LED_OFF); // visualize successful wipe
      delay(200);
      digitalWrite(redLed, LED_ON);
      delay(200);
      digitalWrite(redLed, LED_OFF);
      delay(200);
      digitalWrite(redLed, LED_ON);
      delay(200);
      digitalWrite(redLed, LED_OFF);
    }
    else {
      Serial.println("!!! Wiping Cancelled !!!");
      digitalWrite(redLed, LED_OFF);
    }
  }
  //Check if master card defined, if not let user choose a master card
  //This also useful to just redefine Master Card
  //You can keep other EEPROM records just write other than 1 to EEPROM address 1
  if (EEPROM.read(1) != 143) {  // Look EEPROM if Master Card defined, EEPROM address 1 holds if defined
  // 143 our magical number
    Serial.println("No Master Card Defined");
    Serial.println("Scan A PICC to Define as Master Card");
    do {
      successRead = getID(); // sets successRead to 1 when we get read from reader otherwise 0
      digitalWrite(blueLed, LED_ON); // Visualize Master Card need to be defined
      delay(200);
      digitalWrite(blueLed, LED_OFF);
      delay(200);
    }
    while (!successRead); //the program will not go further while you not get a successful read
    for ( int j = 0; j < 4; j++ ) { // Loop 4 times
      EEPROM.write( 2 +j, readCard[j] ); // Write scanned PICC's UID to EEPROM, start from address 3
    }
    EEPROM.write(1,143); //Write to EEPROM we defined Master Card.
    Serial.println("Master Card Defined");
  }
  Serial.println("##### RFID Door Acces Control v2.0.8 #####"); //For debug purposes
  Serial.println("Master Card's UID");
  for ( int i = 0; i < 4; i++ ) {     // Read Master Card's UID from EEPROM
    masterCard[i] = EEPROM.read(2+i); // Write it to masterCard
    Serial.print(masterCard[i], HEX);
  }
  Serial.println("");
  Serial.println("Waiting PICCs to bo scanned :)");
  cycleLeds();    // Everything ready lets give user some feedback by cycling leds
}


///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop () {
  do {
    successRead = getID(); // sets successRead to 1 when we get read from reader otherwise 0
    if (programMode) {
      cycleLeds(); // Program Mode cycles through RGB waiting to read a new card
    }
    else {
      normalModeOn(); // Normal mode, blue Power LED is on, all others are off
    }
  }
  while (!successRead); //the program will not go further while you not get a successful read
  if (programMode) {
    if ( isMaster(readCard) ) {  //If master card scanned again exit program mode
      Serial.println("This is Master Card"); 
      Serial.println("Exiting Program Mode");
      Serial.println("-----------------------------");
      programMode = false;
      return;
    }
    else {	
      if ( findID(readCard) ) { //If scanned card is known delete it
        Serial.println("I know this PICC, so removing");
        deleteID(readCard);
        Serial.println("-----------------------------");
      }
      else {                    // If scanned card is not known add it
        Serial.println("I do not know this PICC, adding...");
        writeID(readCard);
        Serial.println("-----------------------------");
      }
    }
  }
  else {
    if ( isMaster(readCard) ) {  // If scanned card's ID matches Master Card's ID enter program mode
      programMode = true;
      Serial.println("Hello Master - Entered Program Mode");
      int count = EEPROM.read(0); // Read the first Byte of EEPROM that
      Serial.print("I have ");    // stores the number of ID's in EEPROM
      Serial.print(count);
      Serial.print(" record(s) on EEPROM");
      Serial.println("");
      Serial.println("Scan a PICC to ADD or REMOVE");
      Serial.println("-----------------------------");
    }
    else {
      if ( findID(readCard) ) {        // If not, see if the card is in the EEPROM 
        Serial.println("Welcome, You shall pass");
        openDoor(300);                // Open the door lock for 300 ms
      }
      else {				// If not, show that the ID was not valid
        Serial.println("You shall not pass");
        failed(); 
      }
    }
  }
}

///////////////////////////////////////// Get PICC's UID ///////////////////////////////////
int getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return 0;
  }
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
  Serial.println("Scanned PICC's UID:");
  for (int i = 0; i < 4; i++) {  // 
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
  }
  Serial.println("");
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

///////////////////////////////////////// Cycle Leds (Program Mode) ///////////////////////////////////
void cycleLeds() {
  digitalWrite(redLed, LED_OFF); // Make sure red LED is off
  digitalWrite(greenLed, LED_ON); // Make sure green LED is on
  digitalWrite(blueLed, LED_OFF); // Make sure blue LED is off
  delay(200);
  digitalWrite(redLed, LED_OFF); // Make sure red LED is off
  digitalWrite(greenLed, LED_OFF); // Make sure green LED is off
  digitalWrite(blueLed, LED_ON); // Make sure blue LED is on
  delay(200);
  digitalWrite(redLed, LED_ON); // Make sure red LED is on
  digitalWrite(greenLed, LED_OFF); // Make sure green LED is off
  digitalWrite(blueLed, LED_OFF); // Make sure blue LED is off
  delay(200);
}

//////////////////////////////////////// Normal Mode Led  ///////////////////////////////////
void normalModeOn () {
  digitalWrite(blueLed, LED_ON); // Blue LED ON and ready to read card
  digitalWrite(redLed, LED_OFF); // Make sure Red LED is off
  digitalWrite(greenLed, LED_OFF); // Make sure Green LED is off
  digitalWrite(relay, HIGH); // Make sure Door is Locked
}

//////////////////////////////////////// Read an ID from EEPROM //////////////////////////////
void readID( int number ) {
  int start = (number * 4 ) + 2; // Figure out starting position
  for ( int i = 0; i < 4; i++ ) { // Loop 4 times to get the 4 Bytes
    storedCard[i] = EEPROM.read(start+i); // Assign values read from EEPROM to array
  }
}

///////////////////////////////////////// Add ID to EEPROM   ///////////////////////////////////
void writeID( byte a[] ) {
  if ( !findID( a ) ) { // Before we write to the EEPROM, check to see if we have seen this card before!
    int num = EEPROM.read(0); // Get the numer of used spaces, position 0 stores the number of ID cards
    int start = ( num * 4 ) + 6; // Figure out where the next slot starts
    num++; // Increment the counter by one
    EEPROM.write( 0, num ); // Write the new count to the counter
    for ( int j = 0; j < 4; j++ ) { // Loop 4 times
      EEPROM.write( start+j, a[j] ); // Write the array values to EEPROM in the right position
    }
    successWrite();
  }
  else {
    failedWrite();
  }
}

///////////////////////////////////////// Remove ID from EEPROM   ///////////////////////////////////
void deleteID( byte a[] ) {
  if ( !findID( a ) ) { // Before we delete from the EEPROM, check to see if we have this card!
    failedWrite(); // If not
  }
  else {
    int num = EEPROM.read(0); // Get the numer of used spaces, position 0 stores the number of ID cards
    int slot; // Figure out the slot number of the card
    int start;// = ( num * 4 ) + 6; // Figure out where the next slot starts
    int looping; // The number of times the loop repeats
    int j;
    int count = EEPROM.read(0); // Read the first Byte of EEPROM that stores number of cards
    slot = findIDSLOT( a ); //Figure out the slot number of the card to delete
    start = (slot * 4) + 2;
    looping = ((num - slot) * 4);
    num--; // Decrement the counter by one
    EEPROM.write( 0, num ); // Write the new count to the counter
    for ( j = 0; j < looping; j++ ) { // Loop the card shift times
      EEPROM.write( start+j, EEPROM.read(start+4+j)); // Shift the array values to 4 places earlier in the EEPROM
    }
    for ( int k = 0; k < 4; k++ ) { //Shifting loop
      EEPROM.write( start+j+k, 0);
    }
    successDelete();
  }
}

///////////////////////////////////////// Check Bytes   ///////////////////////////////////
boolean checkTwo ( byte a[], byte b[] ) {
  if ( a[0] != NULL ) // Make sure there is something in the array first
    match = true; // Assume they match at first
  for ( int k = 0; k < 4; k++ ) { // Loop 4 times
    if ( a[k] != b[k] ) // IF a != b then set match = false, one fails, all fail
      match = false;
  }
  if ( match ) { // Check to see if if match is still true
    return true; // Return true
  }
  else  {
    return false; // Return false
  }
}

///////////////////////////////////////// Find Slot   ///////////////////////////////////
int findIDSLOT( byte find[] ) {
  int count = EEPROM.read(0); // Read the first Byte of EEPROM that
  for ( int i = 1; i <= count; i++ ) { // Loop once for each EEPROM entry
    readID(i); // Read an ID from EEPROM, it is stored in storedCard[4]
    if( checkTwo( find, storedCard ) ) { // Check to see if the storedCard read from EEPROM
      // is the same as the find[] ID card passed
      return i; // The slot number of the card
      break; // Stop looking we found it
    }
  }
}

///////////////////////////////////////// Find ID From EEPROM   ///////////////////////////////////
boolean findID( byte find[] ) {
  int count = EEPROM.read(0); // Read the first Byte of EEPROM that
  for ( int i = 1; i <= count; i++ ) {  // Loop once for each EEPROM entry
    readID(i); // Read an ID from EEPROM, it is stored in storedCard[4]
    if( checkTwo( find, storedCard ) ) {  // Check to see if the storedCard read from EEPROM
      return true;
      break; // Stop looking we found it
    }
    else {  // If not, return false   
    }
  }
  return false;
}

///////////////////////////////////////// Write Success to EEPROM   ///////////////////////////////////
// Flashes the green LED 3 times to indicate a successful write to EEPROM
void successWrite() {
  digitalWrite(blueLed, LED_OFF); // Make sure blue LED is off
  digitalWrite(redLed, LED_OFF); // Make sure red LED is off
  digitalWrite(greenLed, LED_OFF); // Make sure green LED is on
  delay(200);
  digitalWrite(greenLed, LED_ON); // Make sure green LED is on
  delay(200);
  digitalWrite(greenLed, LED_OFF); // Make sure green LED is off
  delay(200);
  digitalWrite(greenLed, LED_ON); // Make sure green LED is on
  delay(200);
  digitalWrite(greenLed, LED_OFF); // Make sure green LED is off
  delay(200);
  digitalWrite(greenLed, LED_ON); // Make sure green LED is on
  delay(200);
  Serial.println("Succesfully added ID record to EEPROM");
}

///////////////////////////////////////// Write Failed to EEPROM   ///////////////////////////////////
// Flashes the red LED 3 times to indicate a failed write to EEPROM
void failedWrite() {
  digitalWrite(blueLed, LED_OFF); // Make sure blue LED is off
  digitalWrite(redLed, LED_OFF); // Make sure red LED is off
  digitalWrite(greenLed, LED_OFF); // Make sure green LED is off
  delay(200);
  digitalWrite(redLed, LED_ON); // Make sure red LED is on
  delay(200);
  digitalWrite(redLed, LED_OFF); // Make sure red LED is off
  delay(200);
  digitalWrite(redLed, LED_ON); // Make sure red LED is on
  delay(200);
  digitalWrite(redLed, LED_OFF); // Make sure red LED is off
  delay(200);
  digitalWrite(redLed, LED_ON); // Make sure red LED is on
  delay(200);
  Serial.println("Failed! There is something wrong with ID or bad EEPROM");
}

///////////////////////////////////////// Success Remove UID From EEPROM  ///////////////////////////////////
// Flashes the blue LED 3 times to indicate a success delete to EEPROM
void successDelete() {
  digitalWrite(blueLed, LED_OFF); // Make sure blue LED is off
  digitalWrite(redLed, LED_OFF); // Make sure red LED is off
  digitalWrite(greenLed, LED_OFF); // Make sure green LED is off
  delay(200);
  digitalWrite(blueLed, LED_ON); // Make sure blue LED is on
  delay(200);
  digitalWrite(blueLed, LED_OFF); // Make sure blue LED is off
  delay(200);
  digitalWrite(blueLed, LED_ON); // Make sure blue LED is on
  delay(200);
  digitalWrite(blueLed, LED_OFF); // Make sure blue LED is off
  delay(200);
  digitalWrite(blueLed, LED_ON); // Make sure blue LED is on
  delay(200);
  Serial.println("Succesfully removed ID record from EEPROM");
}

////////////////////// Check readCard IF is masterCard   ///////////////////////////////////
// Check to see if the ID passed is the master programing card
boolean isMaster( byte test[] ) {
  if ( checkTwo( test, masterCard ) )
    return true;
  else
    return false;
}

///////////////////////////////////////// Unlock Door   ///////////////////////////////////
void openDoor( int setDelay ) {
  digitalWrite(blueLed, LED_OFF); // Turn off blue LED
  digitalWrite(redLed, LED_OFF); // Turn off red LED	
  digitalWrite(greenLed, LED_ON); // Turn on green LED
  digitalWrite(relay, LOW); // Unlock door!
  delay(setDelay); // Hold door lock open for given seconds
  digitalWrite(relay, HIGH); // Relock door
  delay(2000); // Hold green LED on for 2 more seconds
}

///////////////////////////////////////// Failed Access  ///////////////////////////////////
void failed() {
  digitalWrite(greenLed, LED_OFF); // Make sure green LED is off
  digitalWrite(blueLed, LED_OFF); // Make sure blue LED is off
  digitalWrite(redLed, LED_ON); // Turn on red LED
  delay(1200);
}
