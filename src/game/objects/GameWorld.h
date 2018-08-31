#ifndef __SDL03__GameWorld__
#define __SDL03__GameWorld__

#include "LoggerCpp.h"

#include "GameParty.h"
#include "GameMonster.h"
#include "GameMap.h"

class GameWorld {
public:
    GameWorld();
    ~GameWorld();
    bool SetStartingPlayerParty();
    bool UnloadPlayerParty();
    bool SetEnemyParty(GameMapEncounterArea* encounterArea);
    bool UnloadEnemyParty();
    bool LoadMap(std::string filename);
    bool UnloadMap();

    GameParty* playerParty;
    GameParty* enemyParty;
    GameMap* currentMap;
};

#endif
