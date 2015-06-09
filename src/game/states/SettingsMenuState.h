#ifndef __SDL03__SettingsMenuState__
#define __SDL03__SettingsMenuState__

#include "GameState.h"

class SettingsMenuState : public GameState {
public:
    SettingsMenuState(std::function<void(GameState*)> callback);
    ~SettingsMenuState();
    GameState* Update(int key);
    GameState* Update(SDL_Event* event);
    std::string ProcessInput(int);
    void Render();

    friend class LuaSettingsMenuState;
protected:
    lua_State *luaState;
};

class LuaSettingsMenuState : public LuaGameState {
public:
    static const char className[];
    static Lunar<LuaSettingsMenuState>::RegType methods[];

    LuaSettingsMenuState(lua_State *L) : LuaGameState(L) {
        this->realObject = (SettingsMenuState*)lua_touserdata(L, 1);
    }

    ~LuaSettingsMenuState() {

    }

    void setObject(lua_State *L) {
        this->realObject = (SettingsMenuState*)lua_touserdata(L, 1);
    }

    int pop(lua_State *L) {
        this->realObject->pop = true;

        return 0;
    }

    int enableRawInput(lua_State *L) {
        this->realObject->acceptRawInput = true;

        return 0;
    }

    int disableRawInput(lua_State *L) {
        this->realObject->acceptRawInput = false;

        return 0;
    }

private:
    SettingsMenuState* realObject;
};


#endif
