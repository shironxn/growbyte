#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <wifimanager.h>

class WiFiModule {
private:
  WIFIMANAGER wifiManager;

public:
  WiFiModule();
  void begin(AsyncWebServer *server);
  bool isConnected();
};
