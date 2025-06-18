#include "DS18B20Sensor.h"
#include <Arduino.h>

DS18B20Sensor::DS18B20Sensor(uint8_t pin)
    : pin(pin), oneWire(pin), sensor(&oneWire) {}

void DS18B20Sensor::begin() {
  sensor.begin();
  sensor.setResolution(10);
}

float DS18B20Sensor::readTemperature() {
  sensor.requestTemperatures();
  while (!sensor.isConversionComplete()) {
    delay(1);
  }
  return sensor.getTempC();
}
