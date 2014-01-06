#ifndef __SDL03__GameMapLayer__
#define __SDL03__GameMapLayer__

#include <vector>

#include "GameObject.h"
#include "GameMapTile.h"
#include "GameMapObject.h"

enum GameMapLayerType {
    Terrain,
    Objects,
    Walkability,
    Effects
};

class GameMapLayer : public GameObject {
public:
    std::string name;
    bool visible;
    GameMapLayerType type;
    std::vector <int> tiles;
    std::vector <GameMapObject*> objects;

    GameMapLayer();
    ~GameMapLayer();
    void Render(int xOffset, int yOffset);
};

#endif
