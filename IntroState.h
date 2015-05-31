#ifndef __SDL03__IntroState__
#define __SDL03__IntroState__

#include "GameState.h"
#include "MainMenuState.h"

class IntroState : public GameState {
public:
    IntroState(std::function<void(GameState*)> callback);
    ~IntroState();
    GameState* Update(int key);
    std::string ProcessInput(int key);
    void Render();
protected:
    lua_State *luaState;
};

#endif
