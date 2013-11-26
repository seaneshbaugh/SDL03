#ifndef __SDL03__GameFont__
#define __SDL03__GameFont__

#include "SDL_ttf.h"

#include "GameResource.h"

class GameFont : public GameResource {
public:
    GameFont();
    ~GameFont();
    bool Load(std::string filename);
    bool Load(std::string filename, int pointSize);
    TTF_Font* font;
private:
    std::string filename;
    int pointSize;
};

#endif
