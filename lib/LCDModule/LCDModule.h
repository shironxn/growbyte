#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class LCDModule {
private:
  uint8_t address, columns, rows;
  LiquidCrystal_I2C lcd;

public:
  LCDModule(uint8_t address, uint8_t columns, uint8_t rows);
  void begin();
  void clear();
  void print(const String &message, uint8_t column, uint8_t row);
};
