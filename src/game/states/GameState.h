#ifndef __SDL03__GameState__
#define __SDL03__GameState__

#include <map>
#include <random>
#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "../../../lib/lua/src/lua.hpp"
#include "../../../lib/lua/src/lua.h"

#include "Locator.h"
#include "GameInputMapper.h"

#include "GameTexture.h"
#include "GameFont.h"
#include "GameSound.h"
#include "GameSong.h"

#include "../parsers/AssetListParser.h"

#include "GameObject.h"
#include "GameText.h"
#include "GameImage.h"

#include "GameParty.h"
#include "GameWorld.h"

class MapState;

enum class GameStateType {
    intro,
    main_menu,
    settings_menu,
    map,
    battle,
    pause_menu
};

class GameState {
public:
    static GameInputMapper* inputMapper;
    static GameWorld* world;
    static std::map<std::string, GameStateType> gameStateTypeMap;

    static MapState* NewGame();
    static GameStateType StateNameToEnum(std::string stateName);

    bool acceptRawInput;
    Log::Logger* logger;

    GameState(std::function<void(GameState*)> callback);
    ~GameState();
    virtual GameState* Update(int key);
    virtual GameState* Update(SDL_Event* event);
    virtual std::string ProcessInput(int key);
    virtual void Render();

    friend class LuaGameState;
protected:
    bool pop;
    std::map <std::string, GameTexture*> textures;
    std::map <std::string, GameSound*> sounds;
    std::map <std::string, GameSong*> songs;
    std::vector <GameText*> texts;

    bool LoadResources(std::string textureListPath, std::string SoundListPath);
    bool LoadTextures(std::string resourceListPath);
    bool LoadSounds(std::string resourceListPath);
    bool LoadSongs(std::string resourceListPath);
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
        std::cout << "Getting texture \"" << textureName << "\"." << std::endl << std::endl;

        std::cout << "Available textures:" << std::endl;
        for (std::map<std::string, GameTexture*>::iterator i = this->realObject->textures.begin(); i != this->realObject->textures.end(); i++) {
            std::cout << (*i).first << std::endl;
        }
        std::cout << std::endl;
        
        GameTexture* texture = this->realObject->textures[textureName];
        
        if (!texture) {
            std::cerr << "Failed to get texture \"" << textureName << "\"." << std::endl;
        }

        lua_pushlightuserdata(L, (void*)texture);

        return 1;
    }

    // TODO: Deprecate this? There's probably no real need to directly access font objects
    // from Lua land. The constructors for lua versions of objects can just take a string
    // as the font name and optionally an int as the font size. Then when the actual object
    // is constructed the combined font name and font size is requested from the font
    // services. The reference to the font can then be held in the C++ object.
    int getFont(lua_State *L) {
        std::string fontName = luaL_checkstring(L, 1);

        int fontSize = GameFont::DEFAULT_FONT_SIZE;

        if (lua_gettop(L) > 1) {
            fontSize = luaL_checkint(L, 2);
        }

        std::shared_ptr<GameFont> font = Services::Locator::FontService()->GetFont(fontName, fontSize);

        //throw;
        lua_pushlightuserdata(L, static_cast<void*>(&font));

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
