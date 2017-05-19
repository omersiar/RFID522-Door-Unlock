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

Arduino IDE:
(Makefile Builds is more convenient and will be available in separate branch)
This project has been tested using Arduino IDE v.1.8.0 and NOT any
other. Please note that the project uses C++11 and therefore an extra
compiling flag has been included in the file ,,platform.local.txt''.
You may need to update your compiler.

EXTERNAL LIBRARIES:
All libraries have been packaged as submodules in the folder "./libraries" and refer
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

Please clone this repos and copy them inside your local sketchbook/libraries folder:
(Something like ,,my/path/to/my/sketchbook/libraries/")
To find the complete path, open Arduino IDE->File->Preferences and look for
,,sketchbook location"

USAGE:
Read HOW_TO_USE.txt

TODOs:
Implementation for further HW, probably using Polymorphism (Virtual classes)
Define more TODOs!
