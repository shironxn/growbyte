#pragma once

#include "DHT.h"
#include <Arduino.h>

class DHTSensor {
private:
  uint8_t pin;
  DHT dht;

public:
  DHTSensor(uint8_t pin, uint8_t type);
  void begin();
  float readTemperature();
  float readHumidity();
};
