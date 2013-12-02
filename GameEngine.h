#ifndef __SDL03__GameEngine__
#define __SDL03__GameEngine__

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "libs/libjson/libjson.h"
#include "GameState.h"
#include "IntroState.h"
#include "GameResource.h"
#include "GameTexture.h"
#include "GameFont.h"
#include "GameSound.h"

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    bool Setup();
    void Start();

private:
    SDL_Window *screen;
    SDL_Renderer *renderer;
    int screenWidth;
    int screenHeight;
    std::string windowTitle;
    std::vector <GameState*> states;
    std::vector <GameTexture*> textures;
    std::vector <GameFont*> fonts;
    std::vector <GameSound*> sounds;

    bool LoadResources();
    bool LoadTextures();
    bool LoadFonts();
    bool LoadSounds();
    void MainLoop();
    void Render();
    bool ReadFile(std::string filename, std::string &contents);
    bool ParseResourceList(std::string jsonString, std::map<std::string, std::string> &resourceList);
};

#endif
