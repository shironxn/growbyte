#include "DHTSensor.h"

DHTSensor::DHTSensor(uint8_t pin, uint8_t type) : pin(pin), dht(pin, type) {}

void DHTSensor::begin() { dht.begin(); }

float DHTSensor::readTemperature() { return dht.readTemperature(); }

float DHTSensor::readHumidity() { return dht.readHumidity(); }
