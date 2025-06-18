#include "WiFiModule.h"
#include <WiFi.h>

WiFiModule::WiFiModule() {}

void WiFiModule::begin(AsyncWebServer *server) {
  wifiManager.startBackgroundTask();
  wifiManager.fallbackToSoftAp(true);
  wifiManager.attachWebServer(server);
  wifiManager.attachUI();
}

bool WiFiModule::isConnected() { return WiFi.isConnected(); }
