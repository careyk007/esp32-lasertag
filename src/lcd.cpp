#include "lcd.h"
#include "SSD1306.h"
#include "pins.h"

SSD1306 lcd(0x3c, LCD_SDA_PIN, LCD_SCL_PIN);

LCD::LCD(void) {

}

LCD::~LCD(void) {

}

void LCD::init(void) {
    lcd.init();
    lcd.flipScreenVertically();
    lcd.setFont(ArialMT_Plain_10);
}

void LCD::runTest(void) {
    lcd.clear();
    lcd.setTextAlignment(TEXT_ALIGN_LEFT);
    lcd.setFont(ArialMT_Plain_10);
    lcd.drawString(0, 0, "Lasertag Mode");
    
    lcd.setFont(ArialMT_Plain_16);
    lcd.drawString(0, 13, "Player : 10");

    lcd.drawHorizontalLine(0, 32, 140);

    lcd.drawString(0, 34, "Team: GREEN");

    lcd.drawHorizontalLine(0, 53, 140);

    lcd.display();
}