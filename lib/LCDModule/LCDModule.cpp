#include "LCDModule.h"

LCDModule::LCDModule(uint8_t address, uint8_t columns, uint8_t rows)
    : address(address), columns(columns), rows(rows),
      lcd(address, columns, rows) {}

void LCDModule::begin() {
  Wire.begin();
  lcd.init();
  lcd.backlight();
}

void LCDModule::clear() { lcd.clear(); }

void LCDModule::print(const String &message, uint8_t column, uint8_t row) {
  lcd.setCursor(column, row);
  lcd.print(message);
}
