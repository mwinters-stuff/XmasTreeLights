
#include <Arduino.h>
#include <ESP8266DebounceSwitch.h>
#include "patterns.h"

Patterns treePatterns(D4,10, NEO_RGB | NEO_KHZ800);
Patterns ringPatterns(D1,24, NEO_GBR | NEO_KHZ800 | PIXEL_FLAG_GEOMETRY_CIRCLE);
ESP8266DebounceSwitch switches;


void onButtonPressed(uint32_t msPressed) {
  if (msPressed > 1000) {
    treePatterns.setRandom();
  } else if (!treePatterns.isOverriding()) {
    treePatterns.setOverride();
  } else {
    treePatterns.incrementOverride();
  }
}

void setup() {
  Serial.begin(115200);

  treePatterns.begin();
  ringPatterns.begin();

  switches.addButtonPin(
      D5, [&](uint8_t pin, uint32_t msPressed) { onButtonPressed(msPressed); },
      true);

  Serial.println("started");

  delay(500);
}

void loop() {
  switches.update();
  treePatterns.update();
  ringPatterns.update();
  delay(10);
}

