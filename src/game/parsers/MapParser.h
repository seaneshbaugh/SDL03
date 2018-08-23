#ifndef __SDL03__MapParser__
#define __SDL03__MapParser__

#include <string>
#include <vector>

#include "../../../lib/libjson/libjson.h"

#include "../objects/GameMap.h"
#include "../objects/GameMapLayer.h"
#include "../objects/GameMapObject.h"
#include "../objects/GameMapTile.h"

class MapParser {
public:
    bool Parse(std::string json, GameMap* map);

private:
    GameMapLayer* ParseLayer(const JSONNode &node);
    std::vector<GameMapLayer*> ParseLayers(const JSONNode &node);
    void ParseLayerData(const JSONNode &node, GameMapLayer* layer);
    void ParseLayerObjects(const JSONNode &node, GameMapLayer* layer);
    std::map<int, GameMapTile*> ParseTileset(const JSONNode &node);
    std::map<int, GameMapTile*> ParseTilesets(const JSONNode &node);
};

#endif
