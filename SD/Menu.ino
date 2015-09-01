void parseMenu(char c) {
  switch (c)  {
    case '1':
      Serial.println(F("Scan a PICC and Enter User Name"));
      while (!Serial.available()) {
        getID();
      }
      if (findID()) { // If not, check if we can find it
        Serial.println(F("This PICC Already Exists"));
        Serial.print(F("User Name: "));
        Serial.println(F("This PICC Already Exists"));
        File dataFile = SD.open(filename);
        if (dataFile) {
          while (dataFile.available()) {
            Serial.write(dataFile.read());
          }
          dataFile.close();
        }
        Serial.println(F("You can change username"));
        return;
      }
      for (byte i = 0; i < 7; i++) {
        username[i] = Serial.read();
      }
      writeID(username);
      break;
    case '2':
      while (!Serial.available())

        break;
    case '3':
      while (!Serial.available())

        break;
    case '4':
      while (!Serial.available()) {

      }
      break;
    case '5':
      programMode = false;
      Serial.println(F("Quit Programming Mode"));
      break;
    case '6':
      SD.remove("/SYS/master.dat");
      checkMaster();
      programMode = false;
      Serial.println(F("Quit Programming Mode"));
      checkMaster();
      break;

  }
}

void printMenu() {
  Serial.println();
  Serial.println(F("////////////////////////////////////////////"));
  Serial.println(F("// Programming Menu                       //"));
  Serial.println(F("////////////////////////////////////////////"));
  Serial.println();
  Serial.println(F("1) Add a PICC"));
  Serial.println(F("2) Remove a PICC"));
  Serial.println(F("3) Change Username"));
  Serial.println(F("4) Set Date and Time /not implemented"));
  Serial.println(F("5) Exit Programming Mode"));
  Serial.println();
  Serial.println(F("6) Change Master Tag"));
  Serial.println(F("X) Delete All Tags /not implemented"));
  Serial.println();
}