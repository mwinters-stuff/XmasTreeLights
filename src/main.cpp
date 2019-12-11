
#include <Arduino.h>
#include <ESP8266DebounceSwitch.h>
#include "patterns.h"

Patterns patterns;
ESP8266DebounceSwitch switches;


void onButtonPressed(uint32_t msPressed) {
  if (msPressed > 1000) {
    patterns.setRandom();
  } else if (!patterns.isOverriding()) {
    patterns.setOverride();
  } else {
    patterns.incrementOverride();
  }
}

void setup() {
  Serial.begin(115200);

  patterns.begin();

  switches.addButtonPin(
      D5, [&](uint8_t pin, uint32_t msPressed) { onButtonPressed(msPressed); },
      true);

  Serial.println("started");

  delay(500);
}

void loop() {
  switches.update();
  patterns.update();
  delay(10);
}

