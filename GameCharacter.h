#ifndef __SDL03__GameCharacter__
#define __SDL03__GameCharacter__

#include <string>
#include <vector>

#include "libs/libjson/libjson.h"

#include "GameObject.h"
#include "GameImage.h"

class GameCharacter : GameObject {
public:
    static SDL_Renderer* renderer;

    std::string name;
    GameTexture* sprite;

    GameCharacter();
    ~GameCharacter();
    unsigned long long GetLevel();
    unsigned long long SetLevel(unsigned long long newLevel);
    unsigned long long GetCurrentHitPoints();
    unsigned long long SetCurrentHitPoints(unsigned long long newCurrentHitPoints);
    unsigned long long GetMaxHitPoints();
    unsigned long long SetMaxHitPoints(unsigned long long newMaxHitPoints);
    unsigned long long GetCurrentMagicPoints();
    unsigned long long SetCurrentMagicPoints(unsigned long long newCurrentMagicPoints);
    unsigned long long GetMaxMagicPoints();
    unsigned long long SetMaxMagicPoints(unsigned long long newMaxMagicPoints);
    unsigned long long Damage(unsigned long long damage);
    bool Load(std::string filename);
    void Render(int x, int y);
protected:
    unsigned long long level;
    unsigned long long currentHitPoints;
    unsigned long long maxHitPoints;
    unsigned long long currentMagicPoints;
    unsigned long long maxMagicPoints;

    bool ParseCharacterFile(std::string jsonString);
};

class LuaGameCharacter {
public:
    static const char className[];
    static Lunar<LuaGameCharacter>::RegType methods[];

    LuaGameCharacter(lua_State *L) {
        int argc = lua_gettop(L);

        if (argc == 1) {
            this->realObject = (GameCharacter*)lua_touserdata(L, 1);
        } else {
            this->realObject = new GameCharacter();
        }
    }

    ~LuaGameCharacter() {
        delete this->realObject;
    }

    int getName(lua_State* L) {
        lua_pushstring(L, this->realObject->name.c_str());

        return 1;
    }

    int setName(lua_State *L) {
        this->realObject->name = luaL_checkstring(L, 1);

        return 0;
    }

    int getLevel(lua_State *L) {
        lua_pushnumber(L, this->realObject->GetLevel());

        return 1;
    }

    int setLevel(lua_State *L) {
        this->realObject->SetLevel(static_cast<unsigned long long>(luaL_checkinteger(L, 1)));

        return 1;
    }

    int getCurrentHitPoints(lua_State *L) {
        lua_pushnumber(L, this->realObject->GetCurrentHitPoints());

        return 1;
    }

    int setCurrentHitPoints(lua_State *L) {
        this->realObject->SetCurrentHitPoints(static_cast<unsigned long long>(luaL_checkinteger(L, 1)));

        return 1;
    }

    int getMaxHitPoints(lua_State *L) {
        lua_pushnumber(L, this->realObject->GetMaxHitPoints());

        return 1;
    }

    int setMaxHitPoints(lua_State *L) {
        this->realObject->SetMaxHitPoints(static_cast<unsigned long long>(luaL_checkinteger(L, 1)));
        
        return 1;
    }

    int getCurrentMagicPoints(lua_State *L) {
        lua_pushnumber(L, this->realObject->GetCurrentMagicPoints());

        return 1;
    }

    int setCurrentMagicPoints(lua_State *L) {
        this->realObject->SetCurrentMagicPoints(static_cast<unsigned long long>(luaL_checkinteger(L, 1)));

        return 1;
    }

    int getMaxMagicPoints(lua_State *L) {
        lua_pushnumber(L, this->realObject->GetMaxMagicPoints());

        return 1;
    }

    int setMaxMagicPoints(lua_State *L) {
        this->realObject->SetMaxMagicPoints(static_cast<unsigned long long>(luaL_checkinteger(L, 1)));

        return 1;
    }

    int damage(lua_State *L) {
        this->realObject->Damage(static_cast<unsigned long long>(luaL_checkinteger(L, 1)));

        return 1;
    }

    int render(lua_State *L) {
        this->realObject->Render(static_cast<int>(luaL_checkinteger(L, 1)), static_cast<int>(luaL_checkinteger(L, 2)));
        
        return 0;
    }
private:
    GameCharacter* realObject;
};

#endif