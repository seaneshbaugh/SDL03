#ifndef __SDL03__GameSound__
#define __SDL03__GameSound__

#include "SDL_mixer.h"

#include "LoggerCpp.h"

#include "GameResource.h"

class GameSound : public GameResource {
public:
    Mix_Chunk* chunk;

    GameSound();
    GameSound(std::string filename);
    ~GameSound();
    bool Load(std::string filename);
private:
    std::string filename;
    Log::Logger* logger;

    bool DestroyChunk();
};

#endif
