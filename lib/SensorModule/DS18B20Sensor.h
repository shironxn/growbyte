#pragma once

#include "DS18B20.h"
#include <Arduino.h>

class DS18B20Sensor {
private:
  uint8_t pin;
  OneWire oneWire;
  DS18B20 sensor;

public:
  DS18B20Sensor(uint8_t pin);
  void begin();
  float readTemperature();
};
