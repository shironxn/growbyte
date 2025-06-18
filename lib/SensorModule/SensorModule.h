#pragma once

#include "DHTSensor.h"
#include "DS18B20Sensor.h"
#include "SoilMoistureSensor.h"

class SensorModule {
private:
  DHTSensor dht;
  DS18B20Sensor ds;
  SoilMoistureSensor soil;

public:
  SensorModule(uint8_t dhtPin, uint8_t dsPin, uint8_t soilMoisturePin);
  void begin();
  float getDHTTemperature();
  float getDHTHumidity();
  float getDS18B20();
  float getSoilMoisture();
};
