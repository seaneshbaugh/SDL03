#ifndef __SDL03__GameSong__
#define __SDL03__GameSong__

#include "GameResource.h"

class GameSong : public GameResource {
public:
    GameSong();
    ~GameSong();
    bool Load(std::string filename);
};

#endif
