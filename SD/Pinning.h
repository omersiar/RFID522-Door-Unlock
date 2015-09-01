/*
	PIN Definitions
*/

#define COMMON_ANODE 		// If Set the LEDs are connected with common anode

#ifdef COMMON_ANODE
#define LED_ON LOW
#define LED_OFF HIGH
#else
#define LED_ON HIGH
#define LED_OFF LOW
#endif

/* 	LED Pins

	For visualizing whats going on hardware
	we need some leds and
	to control door lock a relay and a wipe button
	(or some other hardware)
	Used common anode led,digitalWriting HIGH turns OFF led
	Mind that if you are going to use common cathode led or
	just seperate leds, simply comment out #define COMMON_ANODE,
*/
#define LED_RED 		2	// Set Red Led Pin
#define LED_GREEN	 	6	// Set Green Led Pin
#define LED_BLUE	 	5	// Set Blue Led Pin

/* Output Pins */
#define OUTPUT_PIN 		8	// Set Output Pin (Relay/Servo/...)
#define OUTPUT_ON		HIGH// Set Output ON State
#define OUTPUT_OFF		LOW	// Set Output OFF State

/* 	SPI Slave Pins

	We need to define MFRC522's and SD Card pins and create instance
	Pin layout should be as follows (on Arduino Uno):
	MOSI: Pin 11 / ICSP-4
	MISO: Pin 12 / ICSP-1
	SCK : Pin 13 / ICSP-3

	Mifare RC522 Reset Pin has to be connected to resolv issues with
	broken/incomplete connections to PICCS

*/
#define SD_SS_PIN 		2	// Set Slave Select Pin from SD Card
#define RFID_SS_PIN		10	// Set Slave Select Pin from Mifare RC522 Reader
#define RFID_RST_PIN 	9	// Set Reset Pin from Mifare RC522 Reader

