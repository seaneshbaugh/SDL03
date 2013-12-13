#ifndef __SDL03__MainMenuState__
#define __SDL03__MainMenuState__

#include "GameState.h"
#include "IntroState.h"

class MainMenuState : public GameState {
public:
    MainMenuState(SDL_Renderer* renderer);
    ~MainMenuState();
    GameState* Update(SDL_Event* event);
    std::string ProcessInput(SDL_Event* event);
    void Render();
protected:
    lua_State *luaState;
};

#endif
