#ifndef __SDL03__GameMapEncounterAreaParser__
#define __SDL03__GameMapEncounterAreaParser__

#include "LoggerCpp.h"
#include "../../../lib/libjson/libjson.h"

class GameMapEncounterArea;

class GameMapEncounterAreaParser {
public:
    Log::Logger* logger;

    GameMapEncounterAreaParser();
    ~GameMapEncounterAreaParser();
    bool Parse(std::string json, GameMapEncounterArea* encounterArea);
};

#endif
