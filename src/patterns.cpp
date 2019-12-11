#include <functional>
#include <patterns.h>

Patterns *Patterns::pInstance;

void neoPatternsCallback(NeoPatterns *aLedsPtr) {
  Patterns::instance()->patternsCallback(aLedsPtr);
}

Patterns::Patterns()
    : neoPatterns(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800,
                  neoPatternsCallback) {
  Patterns::pInstance = this;
}

void Patterns::begin() {
  neoPatterns.begin(); // This initializes the NeoPixel library.
  neoPatterns.ColorWipe(COLOR32(0, 0, 02), 50, 0, REVERSE); // light Blue
}

void Patterns::update() { neoPatterns.update(); }

void Patterns::setOverride() {
  Serial.println("Overriding");
  overrideState = sState;
}

void Patterns::incrementOverride() {
  Serial.println("Button Incrementing Override ");

  overrideState++;
  if (overrideState == NUM_SEQUENCES) {
    overrideState = 0;
  }
}

bool Patterns::isOverriding() { return overrideState != NO_OVERRIDE; }

void Patterns::setRandom() {
  Serial.println("Random");

  overrideState = NO_OVERRIDE;
}

void Patterns::patternsCallback(NeoPatterns *aLedsPtr) {

  uint8_t tDuration = random(40, 81);
  uint8_t tColor = random(255);

  if (overrideState != NO_OVERRIDE) {
    Serial.print("Using Override ");
    Serial.println(overrideState);
    sState = overrideState;
  } else {
    int8_t newState = sState;
    while(newState == sState){
      newState = random(NUM_SEQUENCES+1);
    }
    sState = newState;
  }

  Serial.print("Pin=");
  Serial.print(neoPatterns.getPin());
  Serial.print(" Length=");
  Serial.print(neoPatterns.numPixels());
  Serial.print(" State=");
  Serial.print(sState);

  switch (sState) {
  case 0:
    // Cylon
    neoPatterns.ScannerExtended(NeoPatterns::Wheel(tColor), 1, tDuration, 10,
                                FLAG_SCANNER_EXT_CYLON |
                                    FLAG_SCANNER_EXT_VANISH_COMPLETE);
    break;
  case 1:
    // Heartbeat
    neoPatterns.Heartbeat(NeoPatterns::Wheel(tColor), tDuration / 2, 10);
    break;
  case 2:
    // rocket and falling star - 2 times bouncing
    neoPatterns.ScannerExtended(NeoPatterns::Wheel(tColor), 1, tDuration, 10,
                                FLAG_SCANNER_EXT_ROCKET,
                                // | FLAG_SCANNER_EXT_START_AT_BOTH_ENDS,
                                (tDuration & DIRECTION_DOWN));
    break;
  case 3:
    neoPatterns.Stripes(
        NeoPatterns::Wheel(tColor), 1, NeoPatterns::Wheel(tColor + 0x80), 2,
        5 * neoPatterns.numPixels(), tDuration * 2, DIRECTION_DOWN);
    break;
  case 4:

    neoPatterns.Stripes(
        NeoPatterns::Wheel(tColor), 2, NeoPatterns::Wheel(tColor + 0x80), 3,
        5 * neoPatterns.numPixels(), tDuration * 2, DIRECTION_UP);
    break;
  case 5:
    neoPatterns.RainbowCycle(tDuration / 4, (tDuration & DIRECTION_DOWN));
    break;
  case 6:
    neoPatterns.Fade(NeoPatterns::Wheel(tColor),
                     NeoPatterns::Wheel(tColor + 0x80), 64, tDuration);
    break;
  case 7:
    neoPatterns.ColorWipe(NeoPatterns::Wheel(tColor), tDuration);
    break;
  case 8:
    // clear existing color wipe
    neoPatterns.ColorWipe(COLOR32_BLACK, tDuration, FLAG_DO_NOT_CLEAR,
                          DIRECTION_DOWN);
    break;
  case 9:
    // Multiple falling star
    initMultipleFallingStars(&neoPatterns, COLOR32_WHITE_HALF, 7, tDuration / 2,
                             3, neoPatternsCallback);
    break;
  case 10:
    neoPatterns.Fire(tDuration * 2, tDuration / 2);
    // sState = 0; // Start from beginning
    break;
  default:
    Serial.println("ERROR");
    break;
  }

  Serial.print(" ActivePattern=");
  neoPatterns.printPatternName(neoPatterns.ActivePattern, &Serial);
  Serial.print("|");
  Serial.print(neoPatterns.ActivePattern);
  Serial.println();
}