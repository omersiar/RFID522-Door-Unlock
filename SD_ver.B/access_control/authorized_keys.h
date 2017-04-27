#ifndef _AUTHORIZED_KEYS_
#define _AUTHORIZED_KEYS_

#include <MFRC522.h>

const byte authorized_keys [5][4] = { // Authorized tag UIDs [number of tags] [Tag's UID-bytes]
    {00,00,000,00},  // USE the decimal format!!
    {00,00,000,00},
    {00,00,000,00},
    {00,00,000,00},
    {00,00,000,00},
};

const int rows = sizeof(authorized_keys)/sizeof(authorized_keys[0]);
const int cols = sizeof(authorized_keys[0]);

const String names [rows]={ // Corresponding name of each tag/key
    {"JOHN"},
    {"DOE"},
    {"MARCO"},
    {"POLO"},
    {"ARDUINO"},
};


#endif // _AUTHORIZED_KEYS_
