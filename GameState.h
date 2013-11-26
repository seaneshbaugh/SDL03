#ifndef __SDL03__GameState__
#define __SDL03__GameState__

#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"

#include "GameTexture.h"
#include "GameFont.h"
#include "GameSound.h"

class GameState {
public:
    GameState(SDL_Renderer* renderer);
    ~GameState();
    virtual GameState* Update(SDL_Event* event);
    virtual void ProcessInput(SDL_Event* event);
    virtual void RenderObjects(std::vector<GameTexture*> textures, std::vector<GameFont*> fonts, std::vector<GameSound*> sounds);
private:
    SDL_Renderer* renderer;
    bool pop;
};

#endif
