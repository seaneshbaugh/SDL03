#ifndef __SDL03__GameTexture__
#define __SDL03__GameTexture__

#include "GameResource.h"

class GameTexture : public GameResource {
public:
    GameTexture();
    ~GameTexture();
    bool Load(std::string filename);
};

#endif
