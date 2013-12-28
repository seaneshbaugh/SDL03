#ifndef __SDL03__GameMapLayer__
#define __SDL03__GameMapLayer__

#include <vector>

#include "GameObject.h"
#include "GameMapTile.h"

enum GameMapLayerType {
    Terrain,
    Objects,
    Walkability,
    Effects
};

class GameMapLayer : public GameObject {
public:
    bool visible;
    GameMapLayerType type;
    std::vector <int> tiles;

    GameMapLayer();
    ~GameMapLayer();
    void Render(int xOffset, int yOffset);
};

#endif
