Arduino RC522 RFID Door Unlocker
=======

 * Arduino RC522 RFID Door Unlocker
 * July/2014 Omer Siar Baysal
 * 
 * Unlocks a Door (controls a relay actually)
 * using a RC522 RFID reader with SPI interface on your Arduino
 * You define a Master Card which is act as Programmer
 * then you can able to choose card holders who able to unlock
 * the door or not.
 * 
 * Easy User Interface
 *
 * Just one RFID tag needed whether Delete or Add Tags
 * You can choose to use Leds for output or
 * Serial LCD module to inform users. Or you can use both
 *
 * Stores Information on EEPROM
 *
 * Information stored on non volatile memory to preserve
 * Users' tag, Master Card ID will be hard coded on Arduino's Flash
 * No Information lost if power lost. EEPROM has unlimited
 * Read cycle but 100,000 limited Write cycle. 
 * 
 * Security
 * 
 * To keep it simple we are going to use Tag's Unique IDs, 
 * But since this module and Tags are cheap, 
 * I guess their IDs not so Unique.
 *
 * I think it is not secure to only use Tags' UID
 * to verify Tags' users who want to unlock a door.
 *
 * MFRC522 Library also lets us to use some authentication
 * mechanism, writing blocks and reading back
 * and there is great example piece of code
 * about reading and writing PICCs
 * here > http://makecourse.weebly.com/week10segment1.html
 *
 * If you serious about coding and security
 * you should really check Mıfare's datasheet
 * We are going to use NOT so SECURE way to
 * Unlock a door.
 *
 * And there is a note on Datasheet which mentions aboout that
 * Note: The use of Single Size UIDs (unique ones) might end soon, 
 * since the number of usable IDs is limited to approximately 
 * 3.7 billion pieces only. 
 *
 * If you rely on heavy security, figure it out how RFID system
 * can be secure yourself (personally very curious about it)
 *
 * Also there are always security
 * issues if there is a "LOCK" actually.
 * 
 * Credits
 *
 * Omer Siar Baysal who put together this project
 *
 * Idea and most of code from Brett Martin's project
 * http://www.instructables.com/id/Arduino-RFID-Door-Lock/
 * www.pcmofo.com
 *
 * MFRC522 Library
 * https://github.com/miguelbalboa/rfid
 * Based on code Dr.Leong   ( WWW.B2CQSHOP.COM )
 * Created by Miguel Balboa (circuitito.com), Jan, 2012.
 * Rewritten by Søren Thing Andersen (access.thing.dk), fall of 2013 
 * (Translation to English, refactored, comments, anti collision, cascade levels.)
 *
 * Arduino Forum Member luisilva for His Massive Code Correction
 * http://forum.arduino.cc/index.php?topic=257036.0
 * http://forum.arduino.cc/index.php?action=profile;u=198897
 *
 * License
 *
 * You are FREE what to do with this code 
 * Just give credits who put effort on this code
 *
 * "PICC" short for Proximity Integrated Circuit Card (RFID Tags)
 *
