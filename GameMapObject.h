#ifndef __SDL03__GameMapObject__
#define __SDL03__GameMapObject__

#include <map>
#include <string>

#include "GameObject.h"

class GameMapObject : public GameObject {
public:
    int x;
    int y;
    int width;
    int height;
    std::map<std::string, std::string> properties;

    GameMapObject();
    ~GameMapObject();
    SDL_Rect GetPosition();
    void SetPosition(int x, int y);
};

class LuaGameMapObject {
public:
    static const char className[];
    static Lunar<LuaGameMapObject>::RegType methods[];

    LuaGameMapObject(lua_State *L) {
        int argc = lua_gettop(L);

        if (argc == 1) {
            this->realObject = (GameMapObject*)lua_touserdata(L, 1);

            this->createdFromLua = false;
        } else {
            this->realObject = new GameMapObject();

            this->createdFromLua = true;
        }
    }

    ~LuaGameMapObject() {
        if (this->createdFromLua) {
            delete this->realObject;
        }
    }

    int getX(lua_State *L) {
        lua_pushnumber(L, this->realObject->x);

        return 1;
    }

    int getY(lua_State *L) {
        lua_pushnumber(L, this->realObject->y);

        return 1;
    }

    int setPosition(lua_State *L) {
        int x = (int)luaL_checkinteger(L, 1);

        int y = (int)luaL_checkinteger(L, 2);

        this->realObject->SetPosition(x, y);

        return 0;
    }

    int getProperty(lua_State *L) {
        std::string key = std::string(luaL_checkstring(L, 1));

        try {
            lua_pushstring(L, this->realObject->properties.at(key).c_str());
        } catch (const std::out_of_range& exception) {
            lua_pushnil(L);
        }

        return 1;
    }

    int setProperty(lua_State *L) {
        std::string key = std::string(luaL_checkstring(L, 1));

        std::string value = std::string(luaL_checkstring(L, 2));

        this->realObject->properties[key] = value;

        return 0;
    }
private:
    GameMapObject* realObject;
    // Yuck.
    bool createdFromLua;
};


#endif
