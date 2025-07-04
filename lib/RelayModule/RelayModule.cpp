#include "RelayModule.h"
#include <Arduino.h>

RelayModule::RelayModule(uint8_t pin, bool activeLow)
    : relayPin(pin), isActiveLow(activeLow), currentState(false) {}

void RelayModule::begin() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, isActiveLow ? HIGH : LOW);
}

void RelayModule::setState(bool on) {
  this->currentState = on;
  digitalWrite(relayPin, isActiveLow ? (currentState ? LOW : HIGH)
                                     : (currentState ? HIGH : LOW));
}

bool RelayModule::getState() const { return this->currentState; }
