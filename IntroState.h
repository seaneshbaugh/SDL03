#ifndef __SDL03__IntroState__
#define __SDL03__IntroState__

#include "GameState.h"

class IntroState : public GameState {
public:
    IntroState();
    ~IntroState();
    GameState* Update(SDL_Event* event);
    void ProcessInput(SDL_Event* event);
    void RenderObjects();
private:
    bool pop;
};

#endif
