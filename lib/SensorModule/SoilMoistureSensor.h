#pragma once

#include <Arduino.h>

class SoilMoistureSensor {
private:
  uint8_t pin;

public:
  SoilMoistureSensor(uint8_t pin);
  void begin();
  int readMoisture();
};
