///////////////////////////////////////// Write Success to EEPROM   ///////////////////////////////////
// Flashes the green LED n-times to indicate a successful write to EEPROM
void greenBlink(uint8_t n) {
  digitalWrite(LED_BLUE, LED_OFF);   // Make sure blue LED is off
  digitalWrite(LED_RED, LED_OFF);  // Make sure red LED is off
  digitalWrite(LED_GREEN, LED_OFF);  // Make sure green LED is off
  bool state = true;
  for (int i = 0; i < (n*2); i++) {
    digitalWrite(LED_BLUE, state);   // Toggle green LED
    state = !state;
    delay(BLINK_DELAY);
  }
}

///////////////////////////////////////// Write Failed to EEPROM   ///////////////////////////////////
// Flashes the red LED n-times to indicate a failed write to EEPROM
void redBlink(uint8_t n) {
  digitalWrite(LED_BLUE, LED_OFF);   // Make sure blue LED is off
  digitalWrite(LED_RED, LED_OFF);  // Make sure red LED is off
  digitalWrite(LED_GREEN, LED_OFF);  // Make sure green LED is off
  bool state = true;
  for (int i = 0; i < (n*2); i++) {
    digitalWrite(LED_RED, state);   // Toggle red LED
    state = !state;
    delay(BLINK_DELAY);
  }
}

///////////////////////////////////////// Success Remove UID From EEPROM  ///////////////////////////////////
// Flashes the blue LED n-times to indicate a success delete to EEPROM
void blueBlink(uint8_t n) {
  digitalWrite(LED_BLUE, LED_OFF);   // Make sure blue LED is off
  digitalWrite(LED_RED, LED_OFF);  // Make sure red LED is off
  digitalWrite(LED_GREEN, LED_OFF);  // Make sure green LED is off
  bool state = true;
  for (int i = 0; i < (n*2); i++) {
    digitalWrite(LED_BLUE, state);   // Toggle blue LED
    state = !state;
    delay(BLINK_DELAY);
  }
}


///////////////////////////////////////// Cycle Leds (Program Mode) ///////////////////////////////////
void cycleLeds() {
  digitalWrite(LED_RED, LED_OFF); 	// Make sure red LED is off
  digitalWrite(LED_GREEN, LED_ON); 	// Make sure green LED is on
  digitalWrite(LED_BLUE, LED_OFF); 	// Make sure blue LED is off
  delay(CYCLE_DELAY);
  digitalWrite(LED_RED, LED_OFF); 	// Make sure red LED is off
  digitalWrite(LED_GREEN, LED_OFF); 	// Make sure green LED is off
  digitalWrite(LED_BLUE, LED_ON); 	// Make sure blue LED is on
  delay(CYCLE_DELAY);
  digitalWrite(LED_RED, LED_ON); 	// Make sure red LED is on
  digitalWrite(LED_GREEN, LED_OFF); 	// Make sure green LED is off
  digitalWrite(LED_BLUE, LED_OFF); 	// Make sure blue LED is off
  delay(CYCLE_DELAY);               
}

//////////////////////////////////////// Normal Mode Led  ///////////////////////////////////
void blueSolid () {
  digitalWrite(LED_BLUE, LED_ON); 	  // Blue LED ON and ready to read card
  digitalWrite(LED_RED, LED_OFF); 	  // Make sure Red LED is off
  digitalWrite(LED_GREEN, LED_OFF); 	// Make sure Green LED is off
  delay(SOLID_DELAY);               // Hold blue LED on
}

void greenSolid () {
  digitalWrite(LED_BLUE, LED_OFF); 	// Turn off blue LED
  digitalWrite(LED_RED, LED_OFF); 	  // Turn off red LED
  digitalWrite(LED_GREEN, LED_ON); 	// Turn on green LED
  delay(SOLID_DELAY); 				      // Hold green LED on
}

///////////////////////////////////////// Access Denied  ///////////////////////////////////
void redSolid() {
  digitalWrite(LED_GREEN, LED_OFF); 	// Make sure green LED is off
  digitalWrite(LED_BLUE, LED_OFF); 	// Make sure blue LED is off
  digitalWrite(LED_RED, LED_ON); 	  // Turn on red LED
  delay(SOLID_DELAY);               // Hold red LED on
}
