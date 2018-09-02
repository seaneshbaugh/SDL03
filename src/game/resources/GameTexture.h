#ifndef __SDL03__GameTexture__
#define __SDL03__GameTexture__

#include "SDL.h"
#include "SDL_image.h"

#include "LoggerCpp.h"

#include "GameResource.h"

class GameEngine;

class GameTexture : public GameResource {
public:
    SDL_Texture* texture;

    GameTexture();
    GameTexture(std::string filename);
    ~GameTexture();
    bool Load(std::string filename);

private:
    std::string filename;
    Log::Logger* logger;

    bool DestroyTexture();
};

#endif
