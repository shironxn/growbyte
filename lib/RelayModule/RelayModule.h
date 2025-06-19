#pragma once

#include <Arduino.h>

class RelayModule {
private:
  uint8_t relayPin;
  bool isActiveLow;
  bool currentState;

public:
  RelayModule(uint8_t pin, bool activeLow);
  void begin();
  void setState(bool on);
  bool getState() const;
};
