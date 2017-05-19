BOARD_TAG     = mega2560
MONITOR_PORT  = /dev/ttyACM0
ARDUINO_LIBS = Wire SD SPI Sodaq_DS3231/src Adafruit_GFX Adafruit_ST3775 MFRC522/src

LOCAL_CPP_SRCS  ?= $(wildcard ./src/*.cpp)
CPPFLAGS      += -std=c++11 -I./inc/ -I./libs/
USER_LIB_PATH += ./libs
include /usr/share/arduino/Arduino.mk
