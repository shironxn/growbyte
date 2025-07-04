#include "Config.h"
#include "LCDModule.h"
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
LCDModule lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  Serial.begin(115200);

  wifiManager.begin(&server);
  sensors.begin();
  pumpRelay.begin();
  lightRelay.begin();
  lcd.begin();

  web.attachSensor(&sensors);
  web.attachRelays(&pumpRelay, &lightRelay);
  web.setMode("manual");
  web.setSoilThreshold(SOIL_THRESHOLD);
  web.setTempThreshold(TEMP_THRESHOLD);
  web.begin();
}

void loop() {
  float dhtTemp = sensors.getDHTTemperature();
  float dhtHum = sensors.getDHTHumidity();
  float dsTemp = sensors.getDS18B20();
  float soil = sensors.getSoilMoisture();

  lcd.clear();
  lcd.print("ST:" + String((int)dsTemp) + "C AT:" + String((int)dhtTemp) + "C",
            0, 0);
  lcd.print("SH:" + String((int)soil) + "% AH:" + String((int)dhtHum) + "%", 0,
            1);

  if (web.getMode() == "auto") {
    if (!isnan(soil) && !isnan(dsTemp)) {
      if (soil < web.getSoilThreshold() || dsTemp > web.getTempThreshold())
        pumpRelay.setState(true);
    } else {
      pumpRelay.setState(false);
    }
  }

  Serial.println("========== Monitoring ==========");
  Serial.println("Mode         : " + web.getMode());
  Serial.println("Soil Moisture: " + String(soil) + " %");
  Serial.println("DS17B20 Temp : " + String(dsTemp) + " °C");
  Serial.println("DHT Temp     : " + String(dhtTemp) + " °C");
  Serial.println("DHT Humidity : " + String(dhtHum) + " %");
  Serial.println("Pompa        : " +
                 String(pumpRelay.getState() ? "ON" : "OFF"));
  Serial.println("Lampu        : " +

                 String(lightRelay.getState() ? "ON" : "OFF"));
  Serial.println("================================");
  Serial.println();

  delay(500);
}
