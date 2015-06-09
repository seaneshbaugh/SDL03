#ifndef __SDL03__MainMenuState__
#define __SDL03__MainMenuState__

#include "GameState.h"
#include "IntroState.h"
#include "MapState.h"
#include "SettingsMenuState.h"

class MainMenuState : public GameState {
public:
    MainMenuState(std::function<void(GameState*)> callback);
    ~MainMenuState();
    GameState* Update(int key);
    std::string ProcessInput(int key);
    void Render();
protected:
    lua_State *luaState;
};

#endif
