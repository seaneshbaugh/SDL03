#ifndef __SDL03__BattleState__
#define __SDL03__BattleState__

#include <vector>

#include "GameState.h"
#include "GameCharacter.h"
#include "GameMonster.h"
#include "GameMapEncounterArea.h"

class BattleState : public GameState {
public:
    // Okay, this is probably where shared_ptr is useful. This is a BORROWED
    // copy of this pointer. DO NOT DELETE. Ugh, manual memory management.
    // I literally just sighed out loud because I thought about how many
    // memory leaks are probably in this and how my use of pointers is sloppy
    // and egregious.
    GameMapEncounterArea* encounterArea;
    std::vector<GameMonster*> monsters;
    lua_State *luaState;

    BattleState(GameMapEncounterArea* encounterArea, std::function<void(GameState*)> callback);
    ~BattleState();
    GameState* Update(int key);
    std::string ProcessInput(int key);
    void Render();

    friend class MapState;
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

        for (std::vector<GameCharacter*>::iterator character = GameState::world->playerParty->characters.begin(); character != GameState::world->playerParty->characters.end(); character++) {
            lua_pushlightuserdata(L, (void*)(*character));

            lua_rawseti(L, -2, n);

            n += 1;
        }

        return 1;
    }

    int getMonsters(lua_State *L) {
        lua_newtable(L);

        int n = 1;

        for (std::vector<GameCharacter*>::iterator monster = GameState::world->enemyParty->characters.begin(); monster != GameState::world->enemyParty->characters.end(); monster++) {
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
