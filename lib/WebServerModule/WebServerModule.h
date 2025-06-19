#pragma once

#include "RelayModule.h"
#include "SensorModule.h"
#include <ESPAsyncWebServer.h>

class WebServerModule {
private:
  AsyncWebServer *server;
  SensorModule *sensor;
  RelayModule *pumpRelay;
  RelayModule *lightRelay;
  String currentMode = "auto";

public:
  WebServerModule(AsyncWebServer *server);
  void begin();
  void attachSensor(SensorModule *sensor);
  void attachRelays(RelayModule *pump, RelayModule *light);
  void setMode(const String &mode);
  String getMode() const;
};
