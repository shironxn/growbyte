#pragma once

#include <ESPAsyncWebServer.h>
#include <wifimanager.h>

class WiFiModule {
private:
  WIFIMANAGER wifiManager;

public:
  WiFiModule();
  void begin(AsyncWebServer *server);
  bool isConnected();
};
