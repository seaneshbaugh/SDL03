#ifndef __SDL03__GameMapTile__
#define __SDL03__GameMapTile__

#include <string>

#include "GameTexture.h"
#include "GameObject.h"

class GameMapTile: public GameObject {
public:
    int id;
    std::string filename;
    GameTexture* texture;

    GameMapTile();
    ~GameMapTile();
};

#endif
