/* Libraries we are going to use
 *
 */
 
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>

// #include <Servo.h>

/* Servos can lock and unlock door
 * There are examples out there
 */


/* For visualizing whats going on we need some 
 * leds and to control door lock a relay(
 * or some other hardware) 
 * 
 */
 
#define redLed 5
#define greenLed 8
#define blueLed 7
#define relay 6

//define buzzer ? maybe we want that

/* We need to define some boolean ???
 * and string ??? bytes ??
 *
 * I think it is not secure to only use PICC's UID
 * to verify PICC's user who wants to unlock a door
 * MFRC522 Library also let us to use some authentication
 * mechanism, writing blocks and reading back
 * and there is great example piece of code
 * about reading and writing PICCs
 * here > http://makecourse.weebly.com/week10segment1.html
 * If you serious about coding and security
 * you should really check Mıfare's datasheet
 * We going to use completely INSECURE way to
 * Unlock a door. Also there are always security
 * issues if there is a "LOCK" actually.
 *
 */
 
String rfidUID = "";           // We are going to store scanned PICC's UID
String masterUID = "479c85b5"; // Define master PICC's UID


/* We need to define MFRC522's pins and create instance
 * These pins for Uno, look MFRC522 Library for
 * pin configuration for other Arduinos.
 */
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

void setup() {
  
  /* pin config.
   * 
   */
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(relay, OUTPUT);
  
  /*
   * Protocol Config
   */
  Serial.begin(9600);	 // Initialize serial communications with the PC
  SPI.begin();           // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();    // Initialize MFRC522 Hardware
  Serial.println("##### RFID Door Unlocker #####");
  Serial.println("");
  Serial.println("Waiting PICCs to bo scanned :)"); 
}

void loop() {
 
 normalModeOn();
 getUID();
 
 if (rfidUID == masterUID) // Compare UID
 { 
   Serial.println("Oh, Hello Master");
   programModeOn();
   programMode();
 }  
 else {
 checkUID();    // Check if we know this UID or not
 }
  
}  

void getUID() {
  
  /* Getting ready for Reading PICCs
   *
   */
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}
  
  if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}
  // Now we are ready to read PICCs

  
    
    rfidUID = "";      // Emptying String
    for (byte i = 0; i < mfrc522.uid.size; i++) {  // For size of "uid.size" we are going to convert uid.uidByte to string
    rfidUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    rfidUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    
    Serial.println("Scanned PICC UID:");
    Serial.println(rfidUID);
    Serial.println("");
    
    mfrc522.PICC_HaltA();
  
  
}

void programMode() {
  rfidUID = "";     // Emptying String
  Serial.println("Entered Program Mode");

}

void programModeOn() {
  
  digitalWrite(redLed, LOW); // Make sure blue LED is off
  digitalWrite(greenLed, LOW); // Make sure blue LED is off
  digitalWrite(blueLed, HIGH); // Make sure green LED is on
  delay(200);
  
  digitalWrite(redLed, LOW); // Make sure blue LED is off
  digitalWrite(greenLed, HIGH); // Make sure blue LED is on
  digitalWrite(blueLed, LOW); // Make sure green LED is off
  delay(200);
  
  digitalWrite(redLed, HIGH); // Make sure blue LED is on
  digitalWrite(greenLed, LOW); // Make sure blue LED is off
  digitalWrite(blueLed, LOW); // Make sure green LED is off
  delay(200);

}

void normalModeOn () {
  
  
  
void checkUID() {
  
  rfidUID = "";
  getUID();
  
  
}

void unlockDoor() {
  /* How we unlock the door ?
   * Up to you 
   */
  
   
   
  
}
  
