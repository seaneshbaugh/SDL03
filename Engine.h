#ifndef __SDL03__Engine__
#define __SDL03__Engine__

#include <string>
#include "SDL.h"

class Engine {
public:
    Engine();
    ~Engine();
    bool setup();
    void start();

private:
    SDL_Surface *screen;
    int screenWidth;
    int screenHeight;
    int screenBPP;
    std::string windowTitle;
    
    void mainLoop();
    void render();
};

#endif
