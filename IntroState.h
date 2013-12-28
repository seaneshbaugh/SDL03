#ifndef __SDL03__IntroState__
#define __SDL03__IntroState__

#include "GameState.h"
#include "MainMenuState.h"

class IntroState : public GameState {
public:
    IntroState(SDL_Renderer* renderer, std::function<void(GameState*)> callback);
    ~IntroState();
    GameState* Update(SDL_Event* event);
    std::string ProcessInput(SDL_Event* event);
    void Render();
protected:
    lua_State *luaState;
};

#endif
