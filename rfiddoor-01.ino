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
 * Information stored on non volatile memory to preserve
 * Users' tag, Master Card ID will be hard coded on Arduino's Flash
 * No Information lost if power lost. EEPROM has unlimited
 * Read cycle but 100,000 limited Write cycle. 
 * 
 * Security
 *
 * We are going to use Tag's Unique IDs, But since this module
 * and Tags are cheap, I guess their IDs not so Unique.
 *
 * I think it is not secure to only use Tags' UID
 * to verify Tags' users who want to unlock a door.
 *
 * MFRC522 Library also lets us to use some authentication
 * mechanism, writing blocks and reading back
 * and there is great example piece of code
 * about reading and writing PICCs
 * here > http://makecourse.weebly.com/week10segment1.html
 *
 * If you serious about coding and security
 * you should really check Mıfare's datasheet
 * We are going to use completely INSECURE way to
 * Unlock a door.
 *
 * And there is a note on Datasheet which mentions aboout that
 * Note: The use of Single Size UIDs (unique ones) might end soon, 
 * since the number of usable IDs is limited to approximately 
 * 3.7 billion pieces only. 
 *
 * If you rely on heavy security, figure it out how RFID system
 * can be secure yourself (personally very curious about it)
 *
 * Also there are always security
 * issues if there is a "LOCK" actually.
 * 
 * Credits
 *
 * Omer Siar Baysal who put together this project
 *
 * Idea and most of code from Brett Martin's project
 * http://www.instructables.com/id/Arduino-RFID-Door-Lock/
 * www.pcmofo.com
 *
 * MFRC522 Library from miguelbalboa
 * https://github.com/miguelbalboa/rfid
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

#include <EEPROM.h>  // We are going to read and write PICC's UID from/to EEPROM
#include <SPI.h>      // RC522 Module uses SPI protocol
#include <MFRC522.h>   // Library for Mifare RC522 Devices

/* Instead of a Relay maybe you want to use a servo
 * Servos can lock and unlock door locks too
 * There are examples out there, If you want you
 * only need to modify void openDoor
 */

// #include <Servo.h>

/* For visualizing whats going on hardware
 * we need some leds and
 * to control door lock a relay
 * (or some other hardware)
 * Used common anode led,digitalWriting HIGH turns OFF led
 * Care if you are going to use common cathode led
 * Simply comment out #define COMMON_ANODE,
 * If you want to use seperate Leds for each color
 * you also you need to comment
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

/* 
 * buzzer on pin PWM 3 maybe you want that, 
 * if so uncomment this line and at void openDoor
 * and at void failed
 */ 

//#define buzzer 3  

boolean match = false; // initialize card match to false
boolean programMode = false; // initialize programming mode to false

byte storedCard[4];   // Stores an ID read from EEPROM
byte readCard[4];           // Stores scanned ID read from RFID Module
byte masterCard[4] = {0x47,0x9c,0x85,0xb5}; // Define master PICC's UID Here

/* We need to define MFRC522's pins and create instance
 * Pin layout should be as follows (on Arduino Uno):
 * MOSI: Pin 11 / ICSP-4
 * MISO: Pin 12 / ICSP-1
 * SCK : Pin 13 / ISCP-3
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
    /*
     * Arduino Pin Configuration
     */
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(blueLed, OUTPUT);
    pinMode(relay, OUTPUT);
    digitalWrite(relay, HIGH); // Make sure door is locked
    /*
     * Protocol Configuration
     */
    Serial.begin(9600);	 // Initialize serial communications with PC
    SPI.begin();           // MFRC522 Hardware uses SPI protocol
    mfrc522.PCD_Init();    // Initialize MFRC522 Hardware
    Serial.println("##### RFID Door Unlocker #####");
    Serial.println("");
    Serial.println("Waiting PICCs to bo scanned :)");
}

///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop () {
    int successRead;     // this block is only to show to the user the mode
    normalModeOn(); // Normal mode, blue Power LED is on, all others are off
    do {
        successRead = getID(); // Get the ID, sets readCard = to the read ID
        if (programMode) {
        	programModeOn(); // Program Mode cycles through RGB waiting to read a new card
        }
        else {
        	normalModeOn(); // Normal mode, blue Power LED is on, all others are off
        }
    }
    while (!successRead); //the program will not go further while you not get a successful read
		if (programMode) {
			programMode = false;  // next time will enter in normal mode
			if ( isMaster(readCard) ) {  //If master card scanned again exit program mode
                                Serial.println("This is Master Card"); 
				Serial.println("Exiting Program Mode");
                                return;
			}
                        else {	
                                if ( findID(readCard) ) { //If scanned card is known delete it
					Serial.println("I know this PICC, so removing");
					deleteID(readCard);   
					Serial.println("Exiting Program Mode");
				}
				else {                    // If scanned card is not known add it
					Serial.println("I do not know this PICC, adding...");
					writeID(readCard);  
					Serial.println("Exiting Program Mode");
				}
                        }
		}
    else {
        if ( isMaster(readCard) ) {
        	programMode = true;
        	Serial.println("Hello Master - Entered Program Mode");
            	Serial.println("Scan a PICC to ADD or REMOVE");
            	int count = EEPROM.read(0); // Read the first Byte of EEPROM that
		Serial.print("I have "); // stores the number of ID's in EEPROM
		Serial.print(count);
		Serial.print(" record(s) on EEPROM");
		Serial.println("");
        }
        else {
            if ( findID(readCard) ) {        // If not, see if the card is in the EEPROM 
                Serial.println("Welcome, You shall pass");
                openDoor(300);                // If it is, open the door lock for 300 ms
            }
            else {
                Serial.println("You shall not pass");
                failed(); // If not, show that the ID was not valid
            }
        }
    }
}

///////////////////////////////////////// Get PICC's UID ///////////////////////////////////
int getID() {
    /*
     * Getting ready for Reading PICCs
     */
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
        return 0;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
        return 0;
    }
    // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
    Serial.println("Scanned PICC's UID:");
    for (byte i = 0; i < mfrc522.uid.size; i++) {  // for size of uid.size write uid.uidByte to readCard
        readCard[i] = mfrc522.uid.uidByte[i];
        Serial.print(readCard[i], HEX);
    }
    Serial.println("");
    mfrc522.PICC_HaltA(); // Stop reading
    return 1;
}

///////////////////////////////////////// Program Mode Leds ///////////////////////////////////
void programModeOn() {
    digitalWrite(redLed, LED_OFF); // Make sure blue LED is off
    digitalWrite(greenLed, LED_ON); // Make sure blue LED is on
    digitalWrite(blueLed, LED_OFF); // Make sure green LED is off
    delay(200);
    digitalWrite(redLed, LED_OFF); // Make sure blue LED is off
    digitalWrite(greenLed, LED_OFF); // Make sure blue LED is off
    digitalWrite(blueLed, LED_ON); // Make sure green LED is on
    delay(200);
    digitalWrite(redLed, LED_ON); // Make sure blue LED is on
    digitalWrite(greenLed, LED_OFF); // Make sure blue LED is off
    digitalWrite(blueLed, LED_OFF); // Make sure green LED is off
    delay(200);
}

//////////////////////////////////////// Normal Mode Leds  ///////////////////////////////////
void normalModeOn () {
    digitalWrite(blueLed, LED_ON); // Power pin ON and ready to read card
    digitalWrite(redLed, LED_OFF); // Make sure Green LED is off
    digitalWrite(greenLed, LED_OFF); // Make sure Red LED is off
    digitalWrite(relay, HIGH); // Make sure Door is Locked
}

//////////////////////////////////////// Read an ID from EEPROM //////////////////////////////
void readID( int number ) {
    int start = (number * 4 ) - 3; // Figure out starting position
    //Serial.print("Start: ");
    //Serial.print(start);
    //Serial.print("\n\n");
    for ( int i = 0; i < 4; i++ ) { // Loop 4 times to get the 4 Bytes
        storedCard[i] = EEPROM.read(start+i); // Assign values read from EEPROM to array
        /*
         Serial.print("Read [");
         Serial.print(start+i);
         Serial.print("] [");
         Serial.print(storedCard[i], HEX);
         Serial.print("] \n");
         */
    }
}

///////////////////////////////////////// Add ID to EEPROM   ///////////////////////////////////
void writeID( byte a[] ) {
    if ( !findID( a ) ) { // Before we write to the EEPROM, check to see if we have seen this card before!
        int num = EEPROM.read(0); // Get the numer of used spaces, position 0 stores the number of ID cards
        /*
         Serial.print("Num: ");
         Serial.print(num);
         Serial.print(" \n");
         */
        int start = ( num * 4 ) + 1; // Figure out where the next slot starts
        num++; // Increment the counter by one
        EEPROM.write( 0, num ); // Write the new count to the counter
        for ( int j = 0; j < 4; j++ ) { // Loop 4 times
            EEPROM.write( start+j, a[j] ); // Write the array values to EEPROM in the right position
            /*
             Serial.print("W[");
             Serial.print(start+j);
             Serial.print("] Value [");
             Serial.print(a[j], HEX);
             Serial.print("] \n");
             */
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
        int start;// = ( num * 4 ) + 1; // Figure out where the next slot starts
        int looping; // The number of times the loop repeats
        int j;
        int count = EEPROM.read(0); // Read the first Byte of EEPROM that
        // Serial.print("Count: "); // stores the number of ID's in EEPROM
        // Serial.print(count);
        // Serial.print("\n");
        slot = findIDSLOT( a ); //Figure out the slot number of the card to delete
        start = (slot * 4) - 3;
        looping = ((num - slot) * 4);
        num--; // Decrement the counter by one
        EEPROM.write( 0, num ); // Write the new count to the counter
        for ( j = 0; j < looping; j++ ) { // Loop the card shift times
            EEPROM.write( start+j, EEPROM.read(start+4+j)); // Shift the array values to 4 places earlier in the EEPROM
            /*
             Serial.print("W[");
             Serial.print(start+j);
             Serial.print("] Value [");
             Serial.print(a[j], HEX);
             Serial.print("] \n");
             */
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
        /*
         Serial.print("[");
         Serial.print(k);
         Serial.print("] ReadCard [");
         Serial.print(a[k], HEX);
         Serial.print("] StoredCard [");
         Serial.print(b[k], HEX);
         Serial.print("] \n");
         */
        if ( a[k] != b[k] ) // IF a != b then set match = false, one fails, all fail
            match = false;
    }
    if ( match ) { // Check to see if if match is still true
        //Serial.print("Strings Match! \n");
        return true; // Return true
    }
    else  {
        //Serial.print("Strings do not match \n");
        return false; // Return false
    }
}

///////////////////////////////////////// Find Slot   ///////////////////////////////////
int findIDSLOT( byte find[] ) {
    int count = EEPROM.read(0); // Read the first Byte of EEPROM that
    //Serial.print("EEPROM Records: "); // stores the number of ID's in EEPROM
    //Serial.print(count);
    //Serial.println("");
    for ( int i = 1; i <= count; i++ ) { // Loop once for each EEPROM entry
	readID(i); // Read an ID from EEPROM, it is stored in storedCard[6]
        if( checkTwo( find, storedCard ) ) { // Check to see if the storedCard read from EEPROM
		// is the same as the find[] ID card passed
        	// Serial.print("We have a matched card!!! \n");
        	return i; // The slot number of the card
        	break; // Stop looking we found it
        }
    }
}

///////////////////////////////////////// Find ID From EEPROM   ///////////////////////////////////
boolean findID( byte find[] ) {
    int count = EEPROM.read(0); // Read the first Byte of EEPROM that
    // Serial.print("Count: "); // stores the number of ID's in EEPROM
    // Serial.print(count);
    // Serial.print("\n");
    for ( int i = 1; i <= count; i++ ) {  // Loop once for each EEPROM entry
        readID(i); // Read an ID from EEPROM, it is stored in storedCard[6]
        if( checkTwo( find, storedCard ) ) {  // Check to see if the storedCard read from EEPROM
            // is the same as the find[] ID card passed
            // Serial.println("I have a record for this PICC");
            return true;
            break; // Stop looking we found it
        }
        else {  // If not, return false   
            // Serial.println("No record matched on EEPROM for this PICC");
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
    delay(setDelay); // Hold door lock open for 2 seconds
    digitalWrite(relay, HIGH); // Relock door
    //analogWrite(buzzer, 30); // Buzzer on
    delay(2000); // Hold green LED on for 2 more seconds
    //analogWrite(buzzer, 0); // Buzzer off
    digitalWrite(greenLed, LED_OFF);	// Turn off green LED
}

///////////////////////////////////////// Failed Access  ///////////////////////////////////
void failed() {
    digitalWrite(greenLed, LED_OFF); // Make sure green LED is off
    digitalWrite(blueLed, LED_OFF); // Make sure blue LED is off
    digitalWrite(redLed, LED_ON); // Turn on red LED
    //analogWrite(buzzer, 30); // Buzzer on
    delay(1200);
    //analogWrite(buzzer, 0); // Buzzer off
}
