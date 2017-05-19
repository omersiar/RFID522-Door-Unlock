#include <LCD_module.h>

display::display(Adafruit_ST7735* lcd)
{
    disp_pntr = lcd;
}

void display::initializeTFT_LCD()          // Setup for TFT LCD - GREENTAB*!
{
    disp_pntr->initR(INITR_GREENTAB);      // initialize a ST7735R chip, green tab
                                           // Specific implementation for GREENTAB_LCD_HW, refer to Adafruit library documentation.
    disp_pntr->fillScreen(ST7735_BLACK);   // Clear screen
    disp_pntr->setTextSize(1);
    disp_pntr->setTextColor(ST7735_BLUE);
}

void display::initialize_SD_storage()
{
    if (!SD.begin(SD_CS)) {                // Initialize SD
        disp_pntr->fillScreen(ST7735_BLACK);
        disp_pntr->setTextSize(2);
        disp_pntr->setTextColor(ST7735_RED);
        disp_pntr->setCursor(10, 10);
        disp_pntr->println("ERROR:");
        disp_pntr->setCursor(10,30);
        disp_pntr->println("SD CARD");
        disp_pntr->setCursor(10,50);
        disp_pntr->println("DOES NOT");
        disp_pntr->setCursor(10,70);
        disp_pntr->println("INITIATE");
        disp_pntr->setCursor(10,90);
        disp_pntr->println("CHECK &");
        disp_pntr->setCursor(10,110);
        disp_pntr->println("RESET");
        while(1){}                         // If it breaks here: Something is wrong with the SD card
    }

    if (SD.exists("records.csv")) {
    }
    else {                                 // If file does not exist, create it
        csv_file = SD.open("records.csv", FILE_WRITE);
        csv_file.println ("USER,DATE,MONTH,YEAR,TIME");
        csv_file.close();
    }
}

bool display::validate_card(tag my_tag, timeState* my_time)
{
    disp_pntr->fillRect(25, 30, 100, 10, ST7735_BLACK ); // Clear default status from screen (only)
    if(my_tag.access == 1){
        disp_pntr->setCursor(40, 30);
        disp_pntr->println("WELCOME");
        disp_pntr->fillCircle(60, 120, 20, ST7735_GREEN);
        disp_pntr->setCursor(40, 40);
        disp_pntr->println(my_tag.name);
        digitalWrite(ACC_GRANTED, HIGH);
        digitalWrite(ACC_DENIED, LOW);
        delay (2000);
        disp_pntr->fillRect(00, 30, 110, 21, ST7735_BLACK );
        disp_pntr->fillCircle(60, 120, 20, ST7735_BLACK);
        write_time_register(my_tag.name, my_time);   // Write to SD records
        return true;
    }
    else {
        disp_pntr->setCursor(30, 30);
        disp_pntr->println("ACCESS DENIED");
        disp_pntr->setCursor(30, 40);
        disp_pntr->println("Try again");
        disp_pntr->fillCircle(60, 120, 20, ST7735_RED);
        delay (1000);
        disp_pntr->fillRect(00, 30, 110, 21, ST7735_BLACK );
        disp_pntr->fillCircle(60, 120, 20, ST7735_BLACK);
        return false;
    }
}

void display::access_default_status() {
    disp_pntr->setCursor(25, 30);
    disp_pntr->println("PLACE YOUR TAG");
    digitalWrite(ACC_GRANTED, LOW);
    digitalWrite(ACC_DENIED, HIGH);
}

void display::write_time_register(String name, timeState* myTime) {
    if (SD.exists("records.csv")) {    // Check to see if the file exists:
    }
    else {                             // Error: records.csv doesn't exist
        disp_pntr->fillScreen(ST7735_BLACK);
        disp_pntr->setTextSize(2);
        disp_pntr->setTextColor(ST7735_RED);
        disp_pntr->setCursor(10, 10);
        disp_pntr->println("ERROR:");
        disp_pntr->setCursor(10,30);
        disp_pntr->setTextColor(ST7735_GREEN);
        disp_pntr->setTextSize(1);
        disp_pntr->println("file: records.csv");
        disp_pntr->setCursor(10,50);
        disp_pntr->setTextColor(ST7735_RED);
        disp_pntr->setTextSize(2);
        disp_pntr->println("DOES NOT");
        disp_pntr->setCursor(10,70);
        disp_pntr->println("EXIST");
        disp_pntr->setCursor(10,90);
        disp_pntr->println("CHECK &");
        disp_pntr->setCursor(10,110);
        disp_pntr->println("RESET");
        while(1){} // If it breaks here: Something is wrong with the file initialization prob. Typo!
    }
    csv_file = SD.open("records.csv", FILE_WRITE);
    csv_file.println(name +',' + String(myTime->get_current_time().date(),DEC) +','+ String(myTime->get_current_time().month(),DEC) + ',' + String(myTime->get_current_time().year(),DEC) + ',' + String(myTime->get_current_time().hour(),DEC) + ":" + String(myTime->get_current_time().minute(),DEC) + ":" + String(myTime->get_current_time().second(),DEC));
    csv_file.close();
}

void display::print_date_time (timeState* myTime) {
    if (myTime->get_u_date()){
        disp_pntr->fillRect(40, 00, 70, 7, ST7735_BLACK );      /* Clear screen only when date changes */
        disp_pntr->fillRect(00, 10, 51, 7, ST7735_BLACK );      /* Clear screen only when date changes */
        disp_pntr->setCursor(10, 0);
        disp_pntr->println(String("Date: ") + String(myTime->get_current_time().date(),DEC) +'.'+ String(myTime->get_current_time().month(),DEC) + '.' + String(myTime->get_current_time().year(),DEC));
        disp_pntr->setCursor(10, 10);
        disp_pntr->print(myTime->get_week_day());
    }

    if (myTime->get_u_hour()){
        disp_pntr->fillRect(45, 10, 20, 7, ST7735_BLACK );      /* Clear screen only when hour changes */
        disp_pntr->setCursor(45, 10);
        disp_pntr->print(String(myTime->get_current_time().hour(),DEC) + ":");
    }

    if (myTime->get_u_minute()){
        disp_pntr->fillRect(65, 10, 20, 7, ST7735_BLACK );      /* Clear screen only when minute changes */
        disp_pntr->setCursor(65, 10);
        disp_pntr->print(String(myTime->get_current_time().minute(),DEC) + ':');
    }

    if (myTime->get_u_second()){
        disp_pntr->fillRect(85, 10, 20, 7, ST7735_BLACK );      /* Clear screen only when sec changes */
        disp_pntr->setCursor(85, 10);
        disp_pntr->println(myTime->get_current_time().second(),DEC);
    }
}
