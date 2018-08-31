#ifndef __SDL03__MapParser__
#define __SDL03__MapParser__

#include <string>
#include <vector>

#include "LoggerCpp.h"
#include "../../../lib/libjson/libjson.h"

#include "GameMap.h"
#include "GameMapLayer.h"
#include "GameMapObject.h"
#include "GameMapEncounterArea.h"
#include "GameMapTile.h"

class MapParser {
public:
    MapParser();
    bool Parse(std::string json, GameMap* map);
    Log::Logger* logger;

private:
    GameMapLayer* ParseLayer(const JSONNode &node);
    std::vector<GameMapLayer*> ParseLayers(const JSONNode &node);
    void ParseLayerData(const JSONNode &node, GameMapLayer* layer);
    void ParseLayerObjects(const JSONNode &node, GameMapLayer* layer);
    std::map<int, GameMapTile*> ParseTileset(const JSONNode &node);
    std::map<int, GameMapTile*> ParseTilesets(const JSONNode &node);
};

#endif
