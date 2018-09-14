#ifndef __SDL03__CharacterParser__
#define __SDL03__CharacterParser__

#include "LoggerCpp.h"
#include "../../../lib/libjson/libjson.h"

#include "Locator.h"

class GameCharacter;

class CharacterParser {
public:
    Log::Logger* logger;

    CharacterParser();
    ~CharacterParser();
    bool Parse(std::string jsonString, GameCharacter* character);
};

#endif
