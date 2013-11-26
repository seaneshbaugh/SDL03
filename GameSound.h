#ifndef __SDL03__GameSound__
#define __SDL03__GameSound__

#include "GameResource.h"

class GameSound : public GameResource {
public:
    GameSound();
    ~GameSound();
    bool Load(std::string filename);
};

#endif
