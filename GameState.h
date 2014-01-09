#ifndef __SDL03__GameState__
#define __SDL03__GameState__

#include <fstream>
#include <map>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "libs/libjson/libjson.h"
#include "libs/lua/src/lua.hpp"
#include "libs/lua/src/lua.h"

#include "GameTexture.h"
#include "GameFont.h"
#include "GameSound.h"
#include "GameSong.h"

#include "GameObject.h"
#include "GameText.h"
#include "GameImage.h"

class GameState {
public:
    static SDL_Renderer* renderer;

    GameState(std::function<void(GameState*)> callback);
    ~GameState();
    virtual GameState* Update(SDL_Event* event);
    virtual std::string ProcessInput(SDL_Event* event);
    virtual void Render();

    friend class LuaGameState;
protected:
    bool pop;
    std::map <std::string, GameTexture*> textures;
    std::map <std::string, GameFont*> fonts;
    std::map <std::string, GameSound*> sounds;
    std::map <std::string, GameSong*> songs;
    std::vector <GameText*> texts;

    bool LoadResources(std::string textureListPath, std::string fontListPath, std::string SoundListPath);
    bool LoadTextures(std::string resourceListPath);
    bool LoadFonts(std::string resourceListPath);
    bool LoadSounds(std::string resourceListPath);
    bool LoadSongs(std::string resourceListPath);
    bool ReadFile(std::string filename, std::string &contents);
    bool ParseResourceList(std::string jsonString, std::map<std::string, std::string> &resourceList);
};

class LuaGameState {
public:
    static const char className[];
    static Lunar<LuaGameState>::RegType methods[];

    LuaGameState(lua_State *L) {
        this->realObject = (GameState*)lua_touserdata(L, 1);
    }

    ~LuaGameState() {

    }

    void setObject(lua_State *L) {
        this->realObject = (GameState*)lua_touserdata(L, 1);
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

private:
    GameState* realObject;
};

#endif
