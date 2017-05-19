#ifndef _RFID_
#define _RFID_

#include <pin_layout.h>
#include <MFRC522.h>
#include <authorized_keys.h>

typedef struct {String name; bool access;} tag; // Declaration of tag info structure

class rfid_unit{
public:
    rfid_unit(MFRC522* my_rfid);
    tag  check_acces();
    bool check_new_card();
    void clear_nuid();
    void initializeRFID();
    void RFID_Scan();

private:
    MFRC522* rfid_pntr;
    byte nuidPICC [cols]; // Declaration of global old UID-Key (state memory)
};




#endif // _RFID_
