#ifndef __SDL03__GameMap__
#define __SDL03__GameMap__

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "../../../../lib/libjson/libjson.h"
#include "LoggerCpp.h"

#include "FileSystemHelpers.h"
#include "PathHelpers.h"

#include "GameObject.h"
#include "GameMapLayer.h"
#include "GameMapTile.h"
#include "GameMapEncounterArea.h"
#include "GameMapLoadPoint.h"

class MapParser;

class GameMap : public GameObject {
public:
    static SDL_Renderer* renderer;

    std::string name;
    std::string filename;
    int width;
    int height;
    int tilewidth;
    int tileheight;
    std::vector <GameMapLayer*> layers;
    std::map <int, GameMapTile*> tiles;
    std::map <std::string, GameTexture*> textures;
    GameMapLayer* walkabilityLayer;
    Log::Logger* logger;
    
    GameMap();
    GameMap(std::string filename);
    ~GameMap();
    bool ReadFile(std::string filename, std::string &contents);
    bool ParseMapFile(std::string jsonString);
    bool LoadTextures();
    bool Load(std::string filename);
    void Render(int xOffset, int yOffset, int xMovementOffset, int yMovementOffset);
    bool GetWalkability(int x, int y);
    std::vector <GameMapObject*> GetObjects(int x, int y);
private:
    // Meh.
    void SetNameFromFilename();
};


class LuaGameMap {
public:
    static const char className[];
    static Lunar<LuaGameMap>::RegType methods[];

    Log::Logger* logger;

    LuaGameMap(lua_State *L) {
        this->logger = new Log::Logger("lua");

        const int argc = lua_gettop(L);

        if (argc == 1) {
            this->realObject = (GameMap*)lua_touserdata(L, 1);
        } else {
            this->realObject = new GameMap();
        }
    }

    ~LuaGameMap() {
        delete this->realObject;
    }

    int getWidth(lua_State *L) {
        lua_pushnumber(L, this->realObject->width);

        return 1;
    }

    int getHeight(lua_State *L) {
        lua_pushnumber(L, this->realObject->height);

        return 1;
    }
    
    int render(lua_State *L) {
        const int xOffset = (int)luaL_checkinteger(L, 1);
        const int yOffset = (int)luaL_checkinteger(L, 2);
        const int xMovementOffset = (int)luaL_checkinteger(L, 3);
        const int yMovementOffset = (int)luaL_checkinteger(L, 4);

        this->realObject->Render(xOffset, yOffset, xMovementOffset, yMovementOffset);

        return 0;
    }

    int getWalkability(lua_State *L) {
        int x = (int)luaL_checkinteger(L, 1);
        int y = (int)luaL_checkinteger(L, 2);

        this->logger->debug() << "getWalkability called for (" << x << ", " << y << ")";

        lua_pushboolean(L, this->realObject->GetWalkability(x, y));

        return 1;
    }

    int getObjects(lua_State *L) {
        const int x = luaL_checkint(L, 1);

        const int y = (int)luaL_checkint(L, 2);

        this->logger->debug() << "LuaGameMap::getObjects (" << x << ", " << y << ")";

        std::vector<GameMapObject*> result = this->realObject->GetObjects(x, y);

        lua_newtable(L);

        int n = 1;

        for (std::vector<GameMapObject*>::iterator object = result.begin(); object != result.end(); object++) {
            lua_pushlightuserdata(L, (void*)(*object));

            lua_rawseti(L, -2, n);

            n += 1;
        }

        return 1;
    }

private:
    GameMap* realObject;
};

#endif
