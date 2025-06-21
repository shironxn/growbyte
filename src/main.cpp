#include "Config.h"
#include "RelayModule.h"
#include "SensorModule.h"
#include "WebServerModule.h"
#include "WiFiModule.h"
#include <Arduino.h>

AsyncWebServer server(80);
WiFiModule wifiManager;
WebServerModule web(&server);

SensorModule sensors(DHT_PIN, DHT11, DS18B20_PIN, SOIL_PIN);
RelayModule pumpRelay(PUMP_RELAY_PIN, true);
RelayModule lightRelay(LIGHT_RELAY_PIN, true);

void setup() {
  Serial.begin(115200);
  wifiManager.begin(&server);

  sensors.begin();
  pumpRelay.begin();
  lightRelay.begin();

  web.attachSensor(&sensors);
  web.attachRelays(&pumpRelay, &lightRelay);
  web.setMode("auto");

  web.begin();
}

void loop() {
  if (web.getMode() == "auto") {
    float soil = sensors.getSoilMoisture();

    if (!isnan(soil)) {
      if (soil < 50.0) {
        pumpRelay.setState(true);
      } else {
        pumpRelay.setState(false);
      }
    } else {
      Serial.println("Error reading soil moisture");
    }
  }

  delay(5000);
}
