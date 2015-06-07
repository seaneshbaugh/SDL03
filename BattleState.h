#ifndef __SDL03__BattleState__
#define __SDL03__BattleState__

#include <vector>

#include "GameState.h"
#include "GameCharacter.h"
#include "GameMonster.h"

class BattleState : public GameState {
public:
    std::vector<GameMonster*> monsters;
    lua_State *luaState;

    BattleState(std::function<void(GameState*)> callback);
    ~BattleState();
    GameState* Update(int key);
    std::string ProcessInput(int key);
    void Render();

    friend class LuaBattleState;
};

class LuaBattleState : public LuaGameState {
public:
    static const char className[];
    static Lunar<LuaBattleState>::RegType methods[];

    LuaBattleState(lua_State *L) : LuaGameState(L) {
        this->realObject = (BattleState*)lua_touserdata(L, 1);
    }

    ~LuaBattleState() {

    }

    void setObject(lua_State *L) {
        this->realObject = (BattleState*)lua_touserdata(L, 1);
    }

    int pop(lua_State *L) {
        this->realObject->pop = true;

        return 0;
    }

    int getParty(lua_State *L) {
        lua_newtable(L);

        int n = 1;

        for (std::vector<GameCharacter*>::iterator character = GameState::party->characters.begin(); character != GameState::party->characters.end(); character++) {
            lua_pushlightuserdata(L, (void*)(*character));

            lua_rawseti(L, -2, n);

            n += 1;
        }

        return 1;
    }

    int getMonsters(lua_State *L) {
        lua_newtable(L);

        int n = 1;

        for (std::vector<GameMonster*>::iterator monster = this->realObject->monsters.begin(); monster != this->realObject->monsters.end(); monster++) {
            lua_pushlightuserdata(L, (void*)(*monster));

            lua_rawseti(L, -2, n);

            n += 1;
        }

        return 1;
    }

private:
    BattleState* realObject;
};

#endif
