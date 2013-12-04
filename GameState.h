#ifndef __SDL03__GameState__
#define __SDL03__GameState__

#include <fstream>
#include <map>
#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"

#include "libs/libjson/libjson.h"
#include "libs/lua/src/lua.hpp"
#include "libs/lua/src/lua.h"

#include "GameTexture.h"
#include "GameFont.h"
#include "GameSound.h"

#include "GameObject.h"
#include "GameText.h"

class GameState {
public:
    GameState(SDL_Renderer* renderer);
    ~GameState();
    virtual GameState* Update(SDL_Event* event);
    virtual void ProcessInput(SDL_Event* event);
    virtual void Render();
protected:
    SDL_Renderer* renderer;
    bool pop;
    std::map <std::string, GameTexture*> textures;
    std::map <std::string, GameFont*> fonts;
    std::map <std::string, GameSound*> sounds;
    std::vector <GameText*> texts;

    bool LoadResources();
    virtual bool LoadTextures();
    virtual bool LoadFonts();
    virtual bool LoadSounds();
    bool ReadFile(std::string filename, std::string &contents);
    bool ParseResourceList(std::string jsonString, std::map<std::string, std::string> &resourceList);
};

#endif
