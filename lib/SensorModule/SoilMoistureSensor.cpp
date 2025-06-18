#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor(uint8_t pin) : pin(pin) {}

void SoilMoistureSensor::begin() { pinMode(pin, INPUT); }

int SoilMoistureSensor::readMoisture() {
  int rawValue = analogRead(pin);
  rawValue = constrain(rawValue, 0, 4095);

  float percentage = map(rawValue, 4095, 0, 0, 100);
  return percentage;
}
