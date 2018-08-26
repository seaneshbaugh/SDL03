#ifndef __SDL03__GameTexture__
#define __SDL03__GameTexture__

#include "SDL.h"
#include "SDL_image.h"

#include "LoggerCpp.h"

#include "GameResource.h"

class GameTexture : public GameResource {
public:
    static SDL_Renderer* renderer;
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
