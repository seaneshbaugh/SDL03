#ifndef __SDL03__Engine__
#define __SDL03__Engine__

#include <string>
#include <vector>
#include "SDL.h"
#include "GameState.h"
#include "IntroState.h"
#include "GameObject.h"

class Engine {
public:
    Engine();
    ~Engine();
    bool Setup();
    void Start();

private:
    SDL_Surface *screen;
    int screenWidth;
    int screenHeight;
    int screenBPP;
    std::string windowTitle;
    std::vector <GameState*> states;
    
    void MainLoop();
    void Render();
};

#endif
