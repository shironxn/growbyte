#include "WebServerModule.h"
#include "WiFiModule.h"

AsyncWebServer server(80);
WiFiModule wifiManager;
WebServerModule webServer(&server);

void setup() {
  Serial.begin(115200);
  wifiManager.begin(&server);
  webServer.begin();
}

void loop() { delay(500); }
