#pragma once

#include <ESPAsyncWebServer.h>

class WebServerModule {
private:
  AsyncWebServer *server;

public:
  WebServerModule(AsyncWebServer *server);
  void begin();
};
