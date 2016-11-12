# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/) 
and this project adheres to [Semantic Versioning](http://semver.org/).

## [0.1.1] - 2016-11-11
### Added
- Wiping Master Card UID on EEPROM in order to Re-Program on restart

## [0.1.0] - 2015-05-30
### Added
- Initial SD Card Version (stores data on SD Card)
- Arduino Flash optimiziation
- Hardware version check 
- Initial Ethernet version (WebUI)
- Initial No Serial Version

### Changed
- Antenna Gain Control disabled by default (stability concers with Chinese clones)

### Fixed
- Handling PICCs that have 7 bytes UID 

## [0.0.8] - 2014-11-01
### Added
- A script to Delete a known UID from EEPROM

### Changed
- Minor Wipe Mode led behaviour change
- Program Mode no longer exit until Master Card scanned again

## [0.0.7] - 2014-08-27
### Added
- Support for defining Master Card at first use 
(no need to be hard coded on Flash Now)
- Matured Wipe Mode and enabled by default

## [0.0.6] - 2014-08-12
### Added
- Experimental Wipe Mode (Wipe EEPROM during startup)

### Removed
- Gain Control (Now Library supports Gain Control natively)

## [0.0.5] - 2014-08-10
### Changed
- Modified RFID library to support Max. Gain (48db)

## [0.0.4] - 2014-07-27
### Added
- Leds cycle through in Program Mode

### Changed
- Minor delay changes to Leds and Relay

## [0.0.3] - 2014-07-26
### Added
- Support for Common Anode or Cathode Leds
- Better explanation about project

### Changed
- Behaviour on exiting Program Mode (Scan Master Card again to Exit)
- Now PICCs that have 7 bytes UID is handled as they have 4 bytes UID
(Code simply does not care last 3 bytes)

### Removed
- Buzzer support

## [0.0.2] - 2014-07-25
### Added
- RC522 RFID hardware support
- Buzzer support

### Changed
- Better readable code with AStyle

## [0.0.1] - 2014-07-23
### Misc
- Fork of http://www.instructables.com/id/Arduino-RFID-Door-Lock/
- More comments made on Code
- Initial Hardware specific code change (we will use RC522 instead of ID-12)
