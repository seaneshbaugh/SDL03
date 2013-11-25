#ifndef __SDL03__GameState__
#define __SDL03__GameState__

#include "SDL.h"

class GameState {
public:
    GameState();
    virtual ~GameState();
    virtual GameState* Update(SDL_Event* event);
    virtual void ProcessInput(SDL_Event* event);
    virtual void RenderObjects();
private:
    bool pop;
};

#endif
