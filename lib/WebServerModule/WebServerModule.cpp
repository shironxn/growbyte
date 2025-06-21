#include "WebServerModule.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

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

  server->on("/health", HTTP_GET, [](AsyncWebServerRequest *request) {
    DynamicJsonDocument doc(128);
    doc["status"] = "ok";
    doc["uptime"] = millis() / 1000; // waktu uptime dalam detik

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server->on("/data", HTTP_GET, [self](AsyncWebServerRequest *request) {
    DynamicJsonDocument doc(512);
    if (self->sensor) {
      doc["soilMoisture"] = self->sensor->getSoilMoisture();
      doc["ds18b20"] = self->sensor->getDS18B20();
      doc["dht"]["temperature"] = self->sensor->getDHTTemperature();
      doc["dht"]["humidity"] = self->sensor->getDHTHumidity();
    }
    doc["esp32"]["temperature"] = roundf(temperatureRead() * 10.0f) / 10.0f;
    doc["waterPumpSwitch"] =
        self->pumpRelay ? self->pumpRelay->getState() : false;
    doc["lightSwitch"] =
        self->lightRelay ? self->lightRelay->getState() : false;
    doc["mode"] = self->getMode();

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
        Serial.print("Pump switch: ");
        Serial.println(value ? "ON" : "OFF");
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
        Serial.print("Light switch: ");
        Serial.println(value ? "ON" : "OFF");
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
          Serial.print("Mode changed to: ");
          Serial.println(value);
          request->send(200);
        } else {
          request->send(400, "application/json",
                        "{\"error\":\"Invalid mode\"}");
        }
      });

  server->begin();
}

void WebServerModule::setMode(const String &mode) {
  if (mode == "auto" || mode == "manual") {
    this->currentMode = mode;
  }
}

String WebServerModule::getMode() const { return this->currentMode; }

void WebServerModule::attachSensor(SensorModule *sensor) {
  this->sensor = sensor;
}

void WebServerModule::attachRelays(RelayModule *pump, RelayModule *light) {
  this->pumpRelay = pump;
  this->lightRelay = light;
}
