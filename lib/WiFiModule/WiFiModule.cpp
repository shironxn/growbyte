#include "WiFiModule.h"

WiFiModule::WiFiModule() {}

void WiFiModule::begin(AsyncWebServer *server) {
  wifiManager.startBackgroundTask();
  wifiManager.fallbackToSoftAp(true);
  wifiManager.attachWebServer(server);
  wifiManager.attachUI();

  Serial.print("[WIFI] AP created. My IP is: ");
  Serial.println(WiFi.softAPIP());
}

bool WiFiModule::isConnected() { return WiFi.isConnected(); }
