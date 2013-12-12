#ifndef __SDL03__GameEngine__
#define __SDL03__GameEngine__

#include <map>
#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

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

    void MainLoop();
    void Render();
};

#endif
