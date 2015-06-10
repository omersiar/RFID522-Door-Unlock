Arduino RC522 RFID Access Control
=======

Arduino RFID Access Control using a RC522 RFID 
reader with SPI interface on your Arduino
You define a Master Card which is act as Programmer
then you can able to choose card holders 
who will granted access or not

 * **Easy User Interface**

Just one RFID tag needed whether Delete or Add Tags
You can choose to use Leds for output or
Serial LCD module to inform users. Or you can use both

 * **Stores Information on EEPROM**

Information stored on non volatile Arduino's EEPROM 
memory to preserve Users' tag and Master Card
No Information lost if power lost. EEPROM has unlimited
Read cycle but 100,000 limited Write cycle. 

 * **Security**

To keep it simple we are going to use Tag's Unique IDs
It's simple and not hacker proof.

 * **Additional Information**

MFRC522 Library also lets us to use some authentication
mechanism, writing blocks and reading back
and there is great example and piece of code
about reading and writing PICCs
here > http://makecourse.weebly.com/week10segment1.html

More info on: 
http://forum.arduino.cc/index.php?topic=256260

 * **Credits**

Omer Siar Baysal who put together this project

Idea and most of code from Brett Martin's project
http://www.instructables.com/id/Arduino-RFID-Door-Lock/

MFRC522 Library
https://github.com/miguelbalboa/rfid
Created by Miguel Balboa (circuitito.com), Jan, 2012.

Arduino Forum Member luisilva for His Massive Code Correction
http://forum.arduino.cc/index.php?topic=257036.0
