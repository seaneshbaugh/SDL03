#ifndef __SDL03__GameMapEncounterArea__
#define __SDL03__GameMapEncounterArea__

#include "FileSystemHelpers.h"
#include "GameMapEncounterAreaParser.h"
#include "GameMapObject.h"

class GameMapEncounterArea : public GameMapObject {
public:
    // TODO: Consider extracting this to its own class.
    std::vector<std::vector<std::string>> mobs;

    GameMapEncounterArea(std::string filename);
    ~GameMapEncounterArea();
};

#endif
