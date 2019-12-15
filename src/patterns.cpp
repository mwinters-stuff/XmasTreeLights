#include <Arduino.h>
#include <ArduinoJson.h>
#include <patterns.h>
#include <algorithm>
#include <functional>
#include <vector>

std::map<uint8_t, Patterns *> Patterns::pInstances;

std::vector<const char *> names{"Cylon",
                                "Rocket",
                                "Falling Star",
                                "Rainbow",
                                "Stripes",
                                "Theatre Chase",
                                "Fade",
                                "Colour Wipe",
                                "Rocket Both Ends",
                                "Heartbeats",
                                "Multiple Falling Stars",
                                "Fire"};

void neoPatternsCallback(NeoPatterns *aLedsPtr) {
  Patterns *instance = Patterns::instance(aLedsPtr->getPin());
  if (instance != nullptr) {
    instance->patternsCallback(aLedsPtr);
  }
}

Patterns::Patterns(uint8_t ledPin, uint8_t numLeds, uint8_t flags) : neoPatterns(numLeds, ledPin, flags, neoPatternsCallback), beingRandom(false) {
  Patterns::pInstances[ledPin] = this;
}

void Patterns::begin() {
  neoPatterns.begin();                                       // This initializes the NeoPixel library.
  neoPatterns.ColorWipe(COLOR32(0, 0, 02), 50, 0, REVERSE);  // light Blue
}

void Patterns::update() { neoPatterns.update(); }

void Patterns::addSequence(uint8_t sequence) { sequences.push_back(sequence); }

void Patterns::addAllSequences() {
  sequences.clear();
  addSequence(SEQUENCE_CYLON);
  addSequence(SEQUENCE_ROCKET);
  addSequence(SEQUENCE_FALLING_STAR);
  addSequence(SEQUENCE_RAINBOW);
  addSequence(SEQUENCE_STRIPES);
  addSequence(SEQUENCE_THEATRE_CHASE);
  addSequence(SEQUENCE_FADE);
  addSequence(SEQUENCE_COLOUR_WIPE);
  addSequence(SEQUENCE_ROCKET_BOTH_ENDS);
  addSequence(SEQUENCE_HEARTBEATS);
  addSequence(SEQUENCE_FALLING_STARS_MULTIPLE);
  addSequence(SEQUENCE_FIRE);
}

void Patterns::setOverride() {
  Serial.print("Overriding ");
  overrideIndex = sequenceIndex;
  Serial.println(overrideIndex);
}

void Patterns::incrementOverride() {
  Serial.print("Button Incrementing Override ");

  overrideIndex++;
  if (overrideIndex == sequences.size()) {
    overrideIndex = 0;
  }
  Serial.println(overrideIndex);
}

bool Patterns::isOverriding() { return overrideIndex != NO_OVERRIDE; }

void Patterns::resetOverride() {
  Serial.println("Reset Override");
  overrideIndex = NO_OVERRIDE;
}

void Patterns::patternsCallback(NeoPatterns *aLedsPtr) {
  uint8_t tDuration = random(40, 81);
  uint8_t tColor = random(255);

  if (overrideIndex != NO_OVERRIDE) {
    Serial.print("Using Override ");
    Serial.println(overrideIndex);
    sequenceIndex = overrideIndex;
  } else {
    if (beingRandom) {
      sequenceIndex = random(sequences.size() + 1);
    } else {
      sequenceIndex++;  // =
      if (sequenceIndex >= sequences.size()) {
        sequenceIndex = 0;
      }
    }
  }

  Serial.print("Pin=");
  Serial.print(neoPatterns.getPin());
  Serial.print(" Length=");
  Serial.print(neoPatterns.numPixels());
  Serial.print(" ");
  Serial.print(sequences.size());
  Serial.print(" Index=");
  Serial.print(sequenceIndex);

  uint8_t sequence = sequences[sequenceIndex];
  Serial.print(" Sequence=");
  Serial.print(sequence);
  Serial.print(" ");
  Serial.print(names[sequence]);

  switch (sequence) {
    case SEQUENCE_CYLON:
      // Cylon 3 times bouncing
      aLedsPtr->ScannerExtended(NeoPatterns::Wheel(tColor), 5, tDuration, 3, FLAG_SCANNER_EXT_CYLON | (tDuration & FLAG_SCANNER_EXT_VANISH_COMPLETE),
                                (tColor & DIRECTION_DOWN));
      break;
    case SEQUENCE_ROCKET:
      // rocket 2 times bouncing
      aLedsPtr->ScannerExtended(NeoPatterns::Wheel(tColor), 7, tDuration, 2,
                                FLAG_SCANNER_EXT_ROCKET | FLAG_SCANNER_EXT_VANISH_COMPLETE | FLAG_DO_NOT_CLEAR, (tColor & DIRECTION_DOWN));
      break;
    case SEQUENCE_FALLING_STAR:
      // 1 times rocket or falling star
      aLedsPtr->ScannerExtended(COLOR32_WHITE_HALF, 7, tDuration / 2, 0, FLAG_SCANNER_EXT_VANISH_COMPLETE, (tColor & DIRECTION_DOWN));
      break;
    case SEQUENCE_RAINBOW:
      // Rainbow cycle
      aLedsPtr->RainbowCycle(tDuration / 4, (tDuration & DIRECTION_DOWN));
      break;
    case SEQUENCE_STRIPES:
      // new Stripes
      aLedsPtr->Stripes(NeoPatterns::Wheel(tColor), 5, NeoPatterns::Wheel(tColor + 0x80), 3, 2 * aLedsPtr->numPixels(), tDuration * 2,
                        (tColor & DIRECTION_DOWN));
      break;
    case SEQUENCE_THEATRE_CHASE:
      // old TheaterChase
      aLedsPtr->Stripes(NeoPatterns::Wheel(tColor), 1, NeoPatterns::Wheel(tColor + 0x80), 2, 2 * aLedsPtr->numPixels(), tDuration * 2,
                        (tColor & DIRECTION_DOWN));
      break;
    case SEQUENCE_FADE:
      // Fade to complement
      aLedsPtr->Fade(NeoPatterns::Wheel(tColor), NeoPatterns::Wheel(tColor + 0x80), 64, tDuration);
      break;
    case SEQUENCE_COLOUR_WIPE:
      // Color wipe DO_NOT_CLEAR
      aLedsPtr->ColorWipe(NeoPatterns::Wheel(tColor), tDuration, FLAG_DO_NOT_CLEAR, (tColor & DIRECTION_DOWN));
      break;
    case SEQUENCE_ROCKET_BOTH_ENDS:
      // rocket start at both end
      aLedsPtr->ScannerExtended(NeoPatterns::Wheel(tColor), 7, tDuration / 2, 3,
                                FLAG_SCANNER_EXT_ROCKET | (tDuration & FLAG_SCANNER_EXT_VANISH_COMPLETE) | FLAG_SCANNER_EXT_START_AT_BOTH_ENDS);
      break;
    case SEQUENCE_HEARTBEATS:
      // 3 Heartbeats
      aLedsPtr->Heartbeat(NeoPatterns::Wheel(tColor), tDuration / 2, 3);
      break;
    case SEQUENCE_FALLING_STARS_MULTIPLE:
      // Multiple falling star
      initMultipleFallingStars(aLedsPtr, COLOR32_WHITE_HALF, 7, tDuration, 3, &neoPatternsCallback);
      break;
    case SEQUENCE_FIRE:
      if ((aLedsPtr->PixelFlags & PIXEL_FLAG_GEOMETRY_CIRCLE) == 0) {
        // Fire
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

  Serial.println();
}

void Patterns::getJson(JsonObject &doc) {
  doc[F("pin")] = neoPatterns.getPin();
  doc[F("leds")] = neoPatterns.numPixels();
  doc[F("current")] = sequences[sequenceIndex];
  doc[F("current_index")] = sequenceIndex;
  doc[F("current_name")] = names.at(sequences[sequenceIndex]);
  doc[F("random")] = beingRandom;
  doc[F("override_index")] = overrideIndex == NO_OVERRIDE ? -1 : overrideIndex;

  JsonArray array = doc.createNestedArray("sequences");  // adoc.to<JsonArray>();
  for (uint8_t value : sequences) {
    array.add(names.at(value));
  }
}

uint8_t Patterns::findSequenceIndex(String name) {
  auto it = std::find_if(std::begin(names), std::end(names), [&name](const char *arg) {
    Serial.print("Compare ");
    Serial.print(arg);
    Serial.print(" to ");
    Serial.println(name);
    return String(arg).equals(name);
  });
  if (it != std::end(names)) {
    return std::distance(names.begin(), it);
  }
  return 0;
}

void Patterns::setJson(const JsonObject &doc) {
  sequenceIndex = doc[F("current_index")];
  overrideIndex = doc[F("override_index")];
  beingRandom = doc[F("random")];
  JsonArrayConst array = doc[F("sequences")];
  sequences.clear();
  for (String obj : array) {
    Serial.println("Finding " + obj);
    sequences.push_back(findSequenceIndex(obj));
  }
}