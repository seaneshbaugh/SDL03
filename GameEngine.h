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
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "sqlite3.h"

#include "FileSystemHelpers.h"
#include "GameSettings.h"
#include "GameState.h"
#include "IntroState.h"
#include "MainMenuState.h"
#include "MapState.h"
#include "GameInputMapper.h"
#include "GameResource.h"
#include "GameTexture.h"
#include "GameFont.h"
#include "GameSound.h"
#include "GameText.h"
#include "GameImage.h"
#include "GameMap.h"
#include "GameMapTile.h"

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
    std::string applicationDataDiretory;
    GameSettings *settings;
    GameInputMapper inputMapper;

    void MainLoop();
    void Render();
};

#endif
