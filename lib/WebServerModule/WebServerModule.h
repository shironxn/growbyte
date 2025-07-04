#pragma once

#include "RelayModule.h"
#include "SensorModule.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

class WebServerModule {
private:
  AsyncWebServer *server;
  SensorModule *sensor;
  RelayModule *pumpRelay;
  RelayModule *lightRelay;

  String currentMode;
  float soilThreshold;
  float tempThreshold;

public:
  WebServerModule(AsyncWebServer *server);

  void begin();
  void attachSensor(SensorModule *sensor);
  void attachRelays(RelayModule *pump, RelayModule *light);

  void setMode(const String &mode);
  String getMode() const;

  void setSoilThreshold(float threshold);
  float getSoilThreshold() const;

  void setTempThreshold(float threshold);
  float getTempThreshold() const;
};
