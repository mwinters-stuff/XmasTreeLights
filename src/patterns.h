#ifndef _PATTERNS_H
#define _PATTERNS_H

#include <Arduino.h>
#include <NeoPatterns.h>

#define NUM_LEDS 5
#define LED_PIN D4
#define NUM_SEQUENCES 10
#define NO_OVERRIDE -1

class Patterns{
  private:
    NeoPatterns neoPatterns;
    static Patterns *pInstance;

    int8_t overrideState = NO_OVERRIDE;
    int8_t sState = -1;

  public:
    Patterns();
    void begin();
    void update();
    void setOverride();
    void incrementOverride();
    bool isOverriding();
    void setRandom();

    void patternsCallback(NeoPatterns *aLedsPtr);

    static Patterns *instance(){
      return Patterns::pInstance;
    }
};

#endif