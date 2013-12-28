#ifndef __SDL03__MapState__
#define __SDL03__MapState__

#include "GameState.h"
#include "GameMap.h"

class MapState : public GameState {
public:
    GameMap* currentMap;
    lua_State *luaState;

    MapState(SDL_Renderer* renderer, std::function<void(GameState*)> callback);
    ~MapState();
    GameState* Update(SDL_Event* event);
    std::string ProcessInput(SDL_Event* event);
    void Render();
    bool LoadMap(std::string filename);

    friend class LuaMapState;
};

class LuaMapState : public LuaGameState {
public:
    static const char className[];
    static Lunar<LuaMapState>::RegType methods[];

    LuaMapState(lua_State *L) : LuaGameState(L) {
        this->realObject = (MapState*)lua_touserdata(L, 1);
    }

    ~LuaMapState() {

    }

    void setObject(lua_State *L) {
        this->realObject = (MapState*)lua_touserdata(L, 1);
    }

    int getTexture(lua_State *L) {
        std::string textureName = luaL_checkstring(L, 1);

        GameTexture* texture = this->realObject->textures[textureName];

        lua_pushlightuserdata(L, (void*)texture);

        return 1;
    }

    int getFont(lua_State *L) {
        std::string fontName = luaL_checkstring(L, 1);

        GameFont* font = this->realObject->fonts[fontName];

        lua_pushlightuserdata(L, (void*)font);

        return 1;
    }

    int getSound(lua_State *L) {
        std::string soundName = luaL_checkstring(L, 1);

        GameSound* sound = this->realObject->sounds[soundName];

        lua_pushlightuserdata(L, (void*)sound);
        
        return 1;
    }

    int getCurrentMap(lua_State *L) {
        lua_pushlightuserdata(L, (void*)this->realObject->currentMap);

        return 1;
    }

    int loadMap(lua_State *L) {
        std::string filename = luaL_checkstring(L, 1);

        bool result = this->realObject->LoadMap(filename);

        lua_pushboolean(L, result);

        return 1;
    }
    
private:
    MapState* realObject;
};

#endif
