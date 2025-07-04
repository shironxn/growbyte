#include "WebServerModule.h"

String formatUptime(unsigned long millis_) {
  unsigned long seconds = millis_ / 1000;
  unsigned int hours = seconds / 3600;
  unsigned int minutes = (seconds % 3600) / 60;
  unsigned int secs = seconds % 60;

  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%02u:%02u:%02u", hours, minutes, secs);
  return String(buffer);
}

WebServerModule::WebServerModule(AsyncWebServer *srv) : server(srv) {}

void WebServerModule::begin() {
  WebServerModule *self = this;

  if (!LittleFS.begin()) {
    Serial.println("LittleFS Mount Failed, formatting...");
    LittleFS.format();
    if (!LittleFS.begin()) {
      Serial.println("Still failed after format.");
      return;
    }
  }

  server->serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server->onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });

  server->on("/data", HTTP_GET, [self](AsyncWebServerRequest *request) {
    DynamicJsonDocument doc(1024); // Tambahkan buffer jika datanya banyak

    float temp = temperatureRead();
    doc["esp32"]["temperature"] =
        isnan(temp) ? 0.0f : roundf(temp * 10.0f) / 10.0f;

    String uptimeStr = formatUptime(millis());
    doc["esp32"]["uptime"] = uptimeStr.length() > 0 ? uptimeStr : "0:00:00";

    if (self->sensor) {
      float soil = self->sensor->getSoilMoisture();
      doc["soilMoisture"] = isnan(soil) ? 0.0f : soil;

      float ds18 = self->sensor->getDS18B20();
      doc["ds18b20"] = isnan(ds18) ? 0.0f : ds18;

      float dhtTemp = self->sensor->getDHTTemperature();
      doc["dht"]["temperature"] = isnan(dhtTemp) ? 0.0f : dhtTemp;

      float dhtHum = self->sensor->getDHTHumidity();
      doc["dht"]["humidity"] = isnan(dhtHum) ? 0.0f : dhtHum;
    }

    doc["waterPumpSwitch"] =
        self->pumpRelay ? self->pumpRelay->getState() : false;
    doc["lightSwitch"] =
        self->lightRelay ? self->lightRelay->getState() : false;

    doc["mode"] = self->getMode();
    doc["soilThreshold"] = self->getSoilThreshold();
    doc["tempThreshold"] = self->getTempThreshold();

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server->on(
      "/pump-switch", HTTP_POST, [](AsyncWebServerRequest *request) {}, nullptr,
      [self](AsyncWebServerRequest *request, uint8_t *data, size_t len,
             size_t index, size_t total) {
        DynamicJsonDocument doc(256);
        auto error = deserializeJson(doc, data, len);
        if (error) {
          request->send(400, "application/json",
                        "{\"error\":\"Invalid JSON\"}");
          return;
        }
        bool value = doc["value"];
        if (self->pumpRelay)
          self->pumpRelay->setState(value);
        request->send(200);
      });

  server->on(
      "/light-switch", HTTP_POST, [](AsyncWebServerRequest *request) {},
      nullptr,
      [self](AsyncWebServerRequest *request, uint8_t *data, size_t len,
             size_t index, size_t total) {
        DynamicJsonDocument doc(256);
        auto error = deserializeJson(doc, data, len);
        if (error) {
          request->send(400, "application/json",
                        "{\"error\":\"Invalid JSON\"}");
          return;
        }
        bool value = doc["value"];
        if (self->lightRelay)
          self->lightRelay->setState(value);
        request->send(200);
      });

  server->on(
      "/mode", HTTP_POST, [](AsyncWebServerRequest *request) {}, nullptr,
      [self](AsyncWebServerRequest *request, uint8_t *data, size_t len,
             size_t index, size_t total) {
        DynamicJsonDocument doc(256);
        auto error = deserializeJson(doc, data, len);
        if (error) {
          request->send(400, "application/json",
                        "{\"error\":\"Invalid JSON\"}");
          return;
        }
        String value = doc["value"];
        if (value == "auto" || value == "manual") {
          self->setMode(value);
          request->send(200);
        } else {
          request->send(400, "application/json",
                        "{\"error\":\"Invalid mode\"}");
        }
      });

  server->on(
      "/pump-trigger", HTTP_POST, [](AsyncWebServerRequest *request) {},
      nullptr,
      [self](AsyncWebServerRequest *request, uint8_t *data, size_t len,
             size_t index, size_t total) {
        DynamicJsonDocument doc(256);
        auto error = deserializeJson(doc, data, len);
        if (error) {
          request->send(400, "application/json",
                        "{\"error\":\"Invalid JSON\"}");
          return;
        }

        if (doc.containsKey("soilThreshold")) {
          self->setSoilThreshold(doc["soilThreshold"]);
        }

        if (doc.containsKey("tempThreshold")) {
          self->setTempThreshold(doc["tempThreshold"]);
        }

        request->send(200);
      });

  server->begin();
}

void WebServerModule::attachSensor(SensorModule *sensor) {
  this->sensor = sensor;
}

void WebServerModule::attachRelays(RelayModule *pump, RelayModule *light) {
  this->pumpRelay = pump;
  this->lightRelay = light;
}

void WebServerModule::setMode(const String &mode) {
  if (mode == "auto" || mode == "manual") {
    this->currentMode = mode;
  }
}

String WebServerModule::getMode() const { return this->currentMode; }

void WebServerModule::setSoilThreshold(float value) {
  this->soilThreshold = value;
}

float WebServerModule::getSoilThreshold() const { return this->soilThreshold; }

void WebServerModule::setTempThreshold(float value) {
  this->tempThreshold = value;
}

float WebServerModule::getTempThreshold() const { return this->tempThreshold; }
