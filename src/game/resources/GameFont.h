#ifndef __SDL03__GameFont__
#define __SDL03__GameFont__

#include "SDL_ttf.h"

#include "GameResource.h"

class GameFont : public GameResource {
public:
    TTF_Font* font;

    GameFont();
    GameFont(std::string filename);
    GameFont(std::string filename, int pointSize);
    ~GameFont();
    bool Load(std::string filename);
    bool Load(std::string filename, int pointSize);

private:
    std::string filename;
    int pointSize;
};

#endif
