#ifndef __SDL03__GameEngine__
#define __SDL03__GameEngine__

#include <sys/stat.h>
#include <unistd.h>

#include <map>
#include <random>
#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "LoggerCpp.h"
#include "sqlite3.h"

#include "FileSystemHelpers.h"
#include "GameFont.h"
#include "GameImage.h"
#include "GameInputMapper.h"
#include "GameMap.h"
#include "GameMapTile.h"
#include "GameResource.h"
#include "GameSettings.h"
#include "GameSound.h"
#include "GameState.h"
#include "GameText.h"
#include "GameTexture.h"
#include "GameWorld.h"
#include "IntroState.h"
#include "MainMenuState.h"
#include "MapState.h"

class GameEngine {
public:
    static SDL_Renderer* currentRenderer;

    GameEngine();
    ~GameEngine();
    bool Setup();
    void Start();

private:
    SDL_Window* screen;
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
    std::string windowTitle;
    std::vector<GameState*> states;
    std::string applicationDataDiretory;
    std::map<std::string, GameFont*> fonts;
    std::map<std::string, int> fontSizes;
    GameSettings* settings;
    GameInputMapper inputMapper;
    Log::Logger* logger;

    bool SetupLogging();
    bool SetupSDL();
    bool LoadFonts(std::string resourceListPath);
    void MainLoop();
    void Render();
    void DestroyStates();
};

#endif
