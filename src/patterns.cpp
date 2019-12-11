#include <functional>
#include <patterns.h>

std::map<uint8_t, Patterns *> Patterns::pInstances;

void neoPatternsCallback(NeoPatterns *aLedsPtr) {
  Patterns *instance = Patterns::instance(aLedsPtr->getPin());
  if(instance != nullptr){
    instance->patternsCallback(aLedsPtr);

  }
}

Patterns::Patterns(uint8_t ledPin, uint8_t numLeds, uint8_t flags)
    : neoPatterns(numLeds, ledPin, flags,
                  neoPatternsCallback) {
  Patterns::pInstances[ledPin] = this;
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
    sState = random(NUM_SEQUENCES+1);
  }

  Serial.print("Pin=");
  Serial.print(neoPatterns.getPin());
  Serial.print(" Length=");
  Serial.print(neoPatterns.numPixels());
  Serial.print(" State=");
  Serial.print(sState);

  switch (sState) {
    case 0:
        // Cylon 3 times bouncing
        aLedsPtr->ScannerExtended(NeoPatterns::Wheel(tColor), 5, tDuration, 3,
        FLAG_SCANNER_EXT_CYLON | (tDuration & FLAG_SCANNER_EXT_VANISH_COMPLETE), (tColor & DIRECTION_DOWN));
        break;
    case 1:
        // rocket 2 times bouncing
        aLedsPtr->ScannerExtended(NeoPatterns::Wheel(tColor), 7, tDuration, 2,
        FLAG_SCANNER_EXT_ROCKET | FLAG_SCANNER_EXT_VANISH_COMPLETE | FLAG_DO_NOT_CLEAR, (tColor & DIRECTION_DOWN));
        break;
    case 2:
        // 1 times rocket or falling star
        aLedsPtr->ScannerExtended(COLOR32_WHITE_HALF, 7, tDuration / 2, 0, FLAG_SCANNER_EXT_VANISH_COMPLETE,
                (tColor & DIRECTION_DOWN));
        break;
    case 3:
        // Rainbow cycle
        aLedsPtr->RainbowCycle(tDuration / 4, (tDuration & DIRECTION_DOWN));
        break;
    case 4:
        // new Stripes
        aLedsPtr->Stripes(NeoPatterns::Wheel(tColor), 5, NeoPatterns::Wheel(tColor + 0x80), 3, 2 * aLedsPtr->numPixels(),
                tDuration * 2, (tColor & DIRECTION_DOWN));
        break;
    case 5:
        // old TheaterChase
        aLedsPtr->Stripes(NeoPatterns::Wheel(tColor), 1, NeoPatterns::Wheel(tColor + 0x80), 2, 2 * aLedsPtr->numPixels(),
                tDuration * 2, (tColor & DIRECTION_DOWN));
        break;
    case 6:
        // Fade to complement
        aLedsPtr->Fade(NeoPatterns::Wheel(tColor), NeoPatterns::Wheel(tColor + 0x80), 64, tDuration);
        break;
    case 7:
        // Color wipe DO_NOT_CLEAR
        aLedsPtr->ColorWipe(NeoPatterns::Wheel(tColor), tDuration, FLAG_DO_NOT_CLEAR, (tColor & DIRECTION_DOWN));
        break;
    case 8:
        // rocket start at both end
        aLedsPtr->ScannerExtended(NeoPatterns::Wheel(tColor), 7, tDuration / 2, 3,
        FLAG_SCANNER_EXT_ROCKET | (tDuration & FLAG_SCANNER_EXT_VANISH_COMPLETE) | FLAG_SCANNER_EXT_START_AT_BOTH_ENDS);
        break;
    case 9:
        // 3 Heartbeats
        aLedsPtr->Heartbeat(NeoPatterns::Wheel(tColor), tDuration / 2, 3);
        break;
    case 10:
        // Multiple falling star
        initMultipleFallingStars(aLedsPtr, COLOR32_WHITE_HALF, 7, tDuration, 3, &allPatternsRandomHandler);
        break;
    case 11:
        if ((aLedsPtr->PixelFlags & PIXEL_FLAG_GEOMETRY_CIRCLE) == 0) {
            //Fire
            aLedsPtr->Fire(tDuration * 2, tDuration / 2);
        } else {
            // rocket start at both end
            aLedsPtr->ScannerExtended(NeoPatterns::Wheel(tColor), 5, tDuration, 0,
            FLAG_SCANNER_EXT_START_AT_BOTH_ENDS | FLAG_SCANNER_EXT_VANISH_COMPLETE | FLAG_DO_NOT_CLEAR);
        }
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