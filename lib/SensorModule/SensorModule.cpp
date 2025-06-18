#include "SensorModule.h"

SensorModule::SensorModule(uint8_t dhtPin, uint8_t dsPin,
                           uint8_t soilMoisturePin)
    : dht(dhtPin), ds(dsPin), soil(soilMoisturePin) {}

void SensorModule::begin() {
  dht.begin();
  ds.begin();
  soil.begin();
}

float SensorModule::getDHTTemperature() { return dht.readTemperature(); }

float SensorModule::getDHTHumidity() { return dht.readHumidity(); }

float SensorModule::getDS18B20() { return ds.readTemperature(); }

float SensorModule::getSoilMoisture() { return soil.readMoisture(); }
