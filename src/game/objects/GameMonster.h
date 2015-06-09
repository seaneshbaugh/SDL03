#ifndef __SDL03__GameMonster__
#define __SDL03__GameMonster__

#include "GameCharacter.h"

class GameMonster : public GameCharacter {
public:
    GameMonster();
    ~GameMonster();
    void Render(int x, int y);
};

class LuaGameMonster : public LuaGameCharacter {
public:
    static const char className[];
    static Lunar<LuaGameMonster>::RegType methods[];

    LuaGameMonster(lua_State *L) : LuaGameCharacter(L) {
        int argc = lua_gettop(L);

        if (argc == 1) {
            this->realObject = (GameMonster*)lua_touserdata(L, 1);
        } else {
            this->realObject = new GameMonster();
        }
    }

    ~LuaGameMonster() {
        delete this->realObject;
    }

    int getName(lua_State* L) {
        lua_pushstring(L, this->realObject->name.c_str());

        return 1;
    }

    int render(lua_State *L) {
        this->realObject->Render(static_cast<int>(luaL_checkinteger(L, 1)), static_cast<int>(luaL_checkinteger(L, 2)));

        return 0;
    }

private:
    GameMonster* realObject;
};

#endif
