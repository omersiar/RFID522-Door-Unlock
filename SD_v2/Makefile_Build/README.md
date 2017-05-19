HARDWARE REQUIREMENTS:

This project has been implmented with this Hardware, further implementation
for additional HW by future colaborators.

- Arduino Mega2560 like this:
          https://www.arduino.cc/en/Main/ArduinoBoardMega2560

- DS_3231 like this:
          http://howtomechatronics.com/tutorials/arduino/arduino-ds3231-real-time-clock-tutorial/

- SainSmart 1.8 ST7735R TFT LCD Module with MicroSD like this:
          https://www.sainsmart.com/sainsmart-1-8-spi-lcd-module-with-microsd-led-backlight-for-arduino-mega-atmel-atmega.html

- MFRC522 like this:
          http://www.instructables.com/id/Arduino-RFID-Reader-MFRC522-Turorial/

INSTALATION REQUIREMENTS:

arduino-core:
Go to the command line and type:

$ sudo apt-get install arduino-core

arduino-mk:
Go to the command line and type:

$ sudo apt-get install arduino-mk

After you have installed the packages you may need to update your ~/.bashrc file with some new ENV variables.
Please check -> https://github.com/sudar/Arduino-Makefile

Update the local Makefile:

A local Makefile has been shipped within this repo, make sure you update the BOARD_TAG, MONITOR_PORT and the include path of ,,Arduino.mk''.
Please note that the project uses C++11 and therefore CPPFLAGS is appended in the local Makefile before including Arduino.mk.
You may need to update your compiler.

EXTERNAL LIBRARIES:

All libraries have been packaged as submodules in the folder "./libs" and refer
to their original repository. To clone them go to your command line and type:

$ git submodule init && git submodule update

or go to each repo and clone:

- DS_3231 requires this library:
          https://github.com/SodaqMoja/Sodaq_DS3231

- 1.8 ST7735R TFT LCD Module with MicroSD requires this libraries:
          https://github.com/adafruit/Adafruit-GFX-Library
          https://github.com/adafruit/Adafruit-ST7735-Library

- MFRC522 requires this library:
          https://github.com/miguelbalboa/rfid

Please clone this repos and copy them inside your local ./libs folder:
(Something like ,,my/path/to/this/project/libs/")

USAGE:

1- Go to ,,authorized_keys.h" and modify the NUID of your authorized
tags (IN DECIMAL FORMAT) as well as the corresponding Names of each
Tag. More info about how to read a TAG NUID, see the MRFC 522 examples.

2- Open the file ,,./inc/pin_layout.h" and follow or modify the proposed
connections for the HW.

3- Open a terminal and go to the base directory of the Makefile_build i.e.

$ cd /my/path/to/RFID522-Door-Unlock/SD_v2/Makefile_Build

Now compile the source code by typing:

$ make

Finally upload your code to the Arduino Board by typing:

$ make upload

try your RFID cards!

4- Remove the SD card, and read it from a PC, you should find a file
called ,,records.csv". You can open and further use this file in any
spreadsheet program such as ,,LibreOffice Calc".

TODOs:
Implementation for further HW, probably using Polymorphism (Virtual classes)
Define more TODOs!
