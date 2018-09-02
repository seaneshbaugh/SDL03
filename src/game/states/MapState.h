#ifndef __SDL03__MapState__
#define __SDL03__MapState__

#include "../parsers/AssetListParser.h"

#include "GameState.h"
#include "BattleState.h"
#include "GameMap.h"

class MapState : public GameState {
public:
    GameMap* currentMap;
    lua_State* luaState;

    MapState(std::function<void(GameState*)> callback);
    ~MapState();
    GameState* Update(int key);
    std::string ProcessInput(int keyt);
    void Render();
    bool LoadMap(std::string filename);
    bool UnloadMap();

    friend class LuaMapState;
private:
    GameMapEncounterArea* currentEncounterArea;
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

    int pop(lua_State *L) {
        this->realObject->pop = true;

        return 0;
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

    int loadTexture(lua_State *L) {
        std::string textureName = luaL_checkstring(L, 1);

        std::string textureFilename = luaL_checkstring(L, 2);

        GameTexture* texture = new GameTexture();

        if (texture->Load(textureFilename)) {
            this->realObject->textures[textureFilename] = texture;

            lua_pushlightuserdata(L, (void*)texture);
        } else {
            lua_pushnil(L);
        }
        
        return 1;
    }

    int setCurrentEncounterArea(lua_State *L) {
        this->realObject->currentEncounterArea = (GameMapEncounterArea*)lua_touserdata(L, 1);

        return 1;
    }

    int getPlayerSprite(lua_State *L) {
        GameTexture* playerSprite = GameState::world->playerParty->characters[0]->spritesheet;

        if (playerSprite) {
            lua_pushlightuserdata(L, (void*)playerSprite);
        } else {
            throw;
        }

        return 1;
    }
    
private:
    MapState* realObject;
};

#endif
