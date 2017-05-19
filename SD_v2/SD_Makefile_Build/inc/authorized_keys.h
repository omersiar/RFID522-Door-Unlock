#ifndef _AUTHORIZED_KEYS_
#define _AUTHORIZED_KEYS_

const byte authorized_keys [5][4] = { // Authorized tag UIDs [number of tags] [Tag's UID-bytes]
    {38,57,140,24},
    {109,234,7,203},
    {109,234,47,203},
    {109,234,47,20},
    {3,57,140,24},
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
