#include <RFID_module.h>


rfid_unit::rfid_unit(MFRC522* my_rfid){
    rfid_pntr = my_rfid;
}


tag rfid_unit::check_acces() {
    RFID_Scan();
    byte count;
    tag info;
    for (byte n = 0; n < rows; n++) {
        count = 0;
        for (byte i = 0; i < cols; i++) {
            if(rfid_pntr->uid.uidByte[i] == authorized_keys[n][i])
                count ++;
        }
        if (count == cols) {            // If the scanned key matches any of the authorised_keys.h
            info.access = true;         // Grant access
            info.name = names[n];       // Parse User Name
            return info;
        }
        else {
            info.access = false;
        }
    }
    return info;
}

bool rfid_unit::check_new_card() {                       // returns true if the card is different
    for (byte i = 0; i < cols; i++) {
        if(rfid_pntr->uid.uidByte[i] != nuidPICC[i])
        {
            for (byte j = 0; j < cols; j++)
            {            // Store NUID into nuidPICC array
                nuidPICC[j] = rfid_pntr->uid.uidByte[j];
            }
            return 1;
        }
        else {
            return 0;
        }
    }
}

void rfid_unit::clear_nuid() {
    for (byte i = 0; i < cols; i++)
    {
        rfid_pntr->uid.uidByte[i] = 0;
        nuidPICC[i] = 0;
    }
}

void rfid_unit::RFID_Scan() {
    if ( ! rfid_pntr->PICC_IsNewCardPresent())   // Look for new cards
        return;
    if ( ! rfid_pntr->PICC_ReadCardSerial())     // Verify if the NUID has been readed
        return;
    rfid_pntr->PICC_HaltA();                     // Halt PICC
    rfid_pntr->PCD_StopCrypto1();                // Stop encryption on PCD
}

void rfid_unit::initializeRFID() {
    SPI.begin();              // Init SPI bus
    rfid_pntr->PCD_Init();    // Init MFRC522
}
