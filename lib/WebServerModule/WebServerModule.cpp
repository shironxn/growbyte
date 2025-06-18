#include "WebServerModule.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

WebServerModule::WebServerModule(AsyncWebServer *srv) : server(srv) {}

void WebServerModule::begin() {
  if (!LittleFS.begin()) {
    Serial.println("LittleFS Mount Failed, formatting...");
    LittleFS.format();
    if (!LittleFS.begin()) {
      Serial.println("Still failed after format.");
      return;
    }
  }
  Serial.println("LittleFS Mounted");

  server->serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server->onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });

  server->on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    DynamicJsonDocument doc(512);
    doc["soilMoisture"] = 70;
    doc["ds18b20"] = 25.3;
    doc["dht"]["temperature"] = 28.5;
    doc["dht"]["humidity"] = 60.2;
    doc["esp32"]["temperature"] = 45.0;
    doc["waterPumpSwitch"] = true;
    doc["lightSwitch"] = false;
    doc["mode"] = "auto";

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server->on(
      "/pump-switch", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len,
         size_t index, size_t total) {
        DynamicJsonDocument doc(256);
        DeserializationError error = deserializeJson(doc, data, len);
        if (error) {
          request->send(400, "application/json",
                        "{\"error\":\"Invalid JSON\"}");
          return;
        }
        bool value = doc["value"];
        Serial.print("Pump switch: ");
        Serial.println(value ? "ON" : "OFF");
        request->send(200);
      });

  server->on(
      "/light-switch", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len,
         size_t index, size_t total) {
        DynamicJsonDocument doc(256);
        DeserializationError error = deserializeJson(doc, data, len);
        if (error) {
          request->send(400, "application/json",
                        "{\"error\":\"Invalid JSON\"}");
          return;
        }
        bool value = doc["value"];
        Serial.print("Light switch: ");
        Serial.println(value ? "ON" : "OFF");
        request->send(200);
      });

  server->on(
      "/mode", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len,
         size_t index, size_t total) {
        DynamicJsonDocument doc(256);
        DeserializationError error = deserializeJson(doc, data, len);
        if (error) {
          request->send(400, "application/json",
                        "{\"error\":\"Invalid JSON\"}");
          return;
        }
        String value = doc["value"];
        Serial.print("Mode changed to: ");
        Serial.println(value);
        request->send(200);
      });

  server->begin();
}
