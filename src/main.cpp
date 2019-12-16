
#include <Arduino.h>
#include <ESP8266DebounceSwitch.h>

#include <DNSServer.h>
#ifdef ESP32
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#endif
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
DNSServer dnsServer;
AsyncWebServer server(80);

#include "patterns.h"

Patterns treePatterns(D4, 10, NEO_RGB | NEO_KHZ800);
Patterns ringPatterns(D1, 24,
                      NEO_GBR | NEO_KHZ800 | PIXEL_FLAG_GEOMETRY_CIRCLE);
ESP8266DebounceSwitch switches;

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request) {
    // request->addInterestingHeader("ANY");
    // Serial.println(request->url());
    return request->url() != "/get" && request->url() != "/set" &&
           !request->url().startsWith("/site");
  }

  void handleRequest(AsyncWebServerRequest *request) {
    request->redirect("/site/index.html");
  }
};

void onButtonPressed(uint32_t msPressed) {
  if (msPressed > 1000) {
    treePatterns.resetOverride();
  } else if (!treePatterns.isOverriding()) {
    treePatterns.setOverride();
  } else {
    treePatterns.incrementOverride();
  }
}

const char *PARAM_MESSAGE = "message";

AsyncJsonResponse *getResponseJson(String what) {
  AsyncJsonResponse *response = new AsyncJsonResponse();

  JsonObject root = response->getRoot();

  root["heap"] = ESP.getFreeHeap();
  root["ssid"] = WiFi.SSID();
  root["ap_address"] = WiFi.softAPIP().toString();

  if (what == "tree") {
    treePatterns.getJson(root);
  } else if (what == "ring") {
    ringPatterns.getJson(root);
  }
  response->setLength();
  return response;
}

void setup() {
  Serial.begin(115200);
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(false);
  ESP.eraseConfig();
  WiFi.softAP("x-mas-lights",NULL,6);
  dnsServer.start(53, "*", WiFi.softAPIP());

  // MDNS.addService("http", "tcp", 80);

  SPIFFS.begin();

  server.addHandler(new CaptiveRequestHandler())
      .setFilter(ON_AP_FILTER); // only when requested from AP

  server
      .on("/get", HTTP_GET,
          [](AsyncWebServerRequest *request) {
            if (request->hasArg("what")) {
              request->send(getResponseJson(request->arg("what")));
            } else {
              request->send(404);
            }
          })
      .setFilter(ON_AP_FILTER);

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler(
      "/set", [](AsyncWebServerRequest *request, JsonVariant &json) {
        const JsonObject &jsonObj = json.as<JsonObject>();
        if (request->hasArg("what")) {
          String what = request->arg("what");

          if (what == "tree") {
            treePatterns.setJson(jsonObj);
          } else if (what == "ring") {
            ringPatterns.setJson(jsonObj);
          }
          request->send(getResponseJson(what));
        } else {
          request->send(404);
        }
      });
  server.addHandler(handler).setFilter(ON_AP_FILTER);

  server.serveStatic("/site", SPIFFS, "/web/").setDefaultFile("index.html").setFilter(ON_AP_FILTER);

  server.begin();

  treePatterns.addAllSequences();

  ringPatterns.addSequence(SEQUENCE_CYLON);
  ringPatterns.addSequence(SEQUENCE_RAINBOW);
  ringPatterns.addSequence(SEQUENCE_STRIPES);
  ringPatterns.addSequence(SEQUENCE_THEATRE_CHASE);
  ringPatterns.addSequence(SEQUENCE_FADE);
  ringPatterns.addSequence(SEQUENCE_COLOUR_WIPE);
  // ringPatterns.addSequence(SEQUENCE_HEARTBEATS);

  treePatterns.setBeingRandom(true);
  treePatterns.begin();
  ringPatterns.begin();

  switches.addButtonPin(
      D5, [&](uint8_t pin, uint32_t msPressed) { onButtonPressed(msPressed); },
      true);

  Serial.println("started " + WiFi.softAPIP().toString());

  delay(500);
}

uint32_t lastms = 0;
void loop() {
  dnsServer.processNextRequest();
  switches.update();
  if (millis() - lastms >= 10) {
    treePatterns.update();
    ringPatterns.update();
    lastms = millis();
  }
}
