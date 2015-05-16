Arduino RC522 RFID Access Control
=======

More info on: 
http://forum.arduino.cc/index.php?topic=256260

July/2014 Omer Siar Baysal

 * **Warning:**
 
With 2.0.5 version you can define master card without need
to be hard coded it's UID. Make sure you wiped EEPROM.

--------

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
It's simple, a bit secure, but not hacker proof.

 * **Additional Information**

MFRC522 Library also lets us to use some authentication
mechanism, writing blocks and reading back
and there is great example and piece of code
about reading and writing PICCs
here > http://makecourse.weebly.com/week10segment1.html

 * **Credits**

Omer Siar Baysal who put together this project

Idea and most of code from Brett Martin's project
http://www.instructables.com/id/Arduino-RFID-Door-Lock/
www.pcmofo.com

MFRC522 Library
https://github.com/miguelbalboa/rfid
Based on code Dr.Leong   ( WWW.B2CQSHOP.COM )
Created by Miguel Balboa (circuitito.com), Jan, 2012.
Rewritten by Søren Thing Andersen (access.thing.dk), fall of 2013 
(Translation to English, refactored, comments, anti collision, cascade levels.)

Arduino Forum Member luisilva for His Massive Code Correction
http://forum.arduino.cc/index.php?topic=257036.0
http://forum.arduino.cc/index.php?action=profile;u=198897

 * **License**

    Arduino RFID Access Control
    Copyright (C) 2015 <Omer Siar Baysal>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
