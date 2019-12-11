#ifndef _PATTERNS_H
#define _PATTERNS_H

#include <Arduino.h>
#include <NeoPatterns.h>
#include <map>

#define NUM_SEQUENCES 11
#define NO_OVERRIDE -1

class Patterns{
  private:
    NeoPatterns neoPatterns;
    static std::map<uint8_t, Patterns *> pInstances;

    int8_t overrideState = NO_OVERRIDE;
    int8_t sState = -1;


  public:
    Patterns(uint8_t ledPin, uint8_t numLeds, uint8_t flags);
    void begin();
    void update();
    void setOverride();
    void incrementOverride();
    bool isOverriding();
    void setRandom();

    void patternsCallback(NeoPatterns *aLedsPtr);

    static Patterns *instance(uint8_t pin){
      return Patterns::pInstances[pin];
    }
};

#endif