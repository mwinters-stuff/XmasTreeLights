#ifndef _PATTERNS_H
#define _PATTERNS_H

#include <Arduino.h>
#include <NeoPatterns.h>
#include <ArduinoJson.h>
#include <map>
#include <vector>

#define NO_OVERRIDE SIZE_MAX

#define SEQUENCE_CYLON 0
#define SEQUENCE_ROCKET 1
#define SEQUENCE_FALLING_STAR 2
#define SEQUENCE_RAINBOW 3
#define SEQUENCE_STRIPES 4
#define SEQUENCE_THEATRE_CHASE 5
#define SEQUENCE_FADE 6
#define SEQUENCE_COLOUR_WIPE 7
#define SEQUENCE_ROCKET_BOTH_ENDS 8
#define SEQUENCE_HEARTBEATS 9
#define SEQUENCE_FALLING_STARS_MULTIPLE 10
#define SEQUENCE_FIRE 11

class Patterns{
  private:
    NeoPatterns neoPatterns;
    static std::map<uint8_t, Patterns *> pInstances;
    std::vector<uint8_t> sequences;

    std::size_t overrideIndex = NO_OVERRIDE;
    std::size_t sequenceIndex = -1;
    bool beingRandom;

    uint8_t findSequenceIndex(String name);
  public:
    Patterns(uint8_t ledPin, uint8_t numLeds, uint8_t flags);
    void begin();
    void update();
    void setOverride();
    void incrementOverride();
    bool isOverriding();
    void resetOverride();

    void patternsCallback(NeoPatterns *aLedsPtr);
    void addSequence(uint8_t sequence);
    void addAllSequences();

    void setBeingRandom(boolean being) { beingRandom = being;}

    static Patterns *instance(uint8_t pin){
      return Patterns::pInstances[pin];
    }

    void getJson(JsonObject& doc);
    void setJson(const JsonObject& doc);
};

#endif