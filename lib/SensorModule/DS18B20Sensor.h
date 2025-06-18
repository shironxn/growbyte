#pragma once

#include "DS18B20.h"
#include <OneWire.h>
#include <sys/types.h>

class DS18B20Sensor {
private:
  u_int8_t pin;
  OneWire oneWire;
  DS18B20 sensor;

public:
  DS18B20Sensor(u_int8_t pin);
  void begin();
  float readTemperature();
};
