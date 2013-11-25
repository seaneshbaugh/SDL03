#include "Engine.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

Engine::Engine() {
    this->screen = NULL;
    
    this->screenWidth = SCREEN_WIDTH;
    
    this->screenHeight = SCREEN_WIDTH;
    
    this->screenBPP = SCREEN_BPP;
    
    this->windowTitle = "Hello, world!";
}

Engine::~Engine() {
    SDL_Quit();
}

bool Engine::setup() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return false;
    }

    this->screen = SDL_SetVideoMode(this->screenWidth, this->screenHeight, this->screenBPP, SDL_SWSURFACE);
    
    if (this->screen == NULL) {
        return false;
    }
    
    SDL_WM_SetCaption(this->windowTitle.c_str(), NULL);

    return true;
}

void Engine::start() {
    this->mainLoop();
}

void Engine::mainLoop() {
    SDL_Event event;

    bool quit = false;
    
    while (quit == false) {
        this->render();
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
}

void Engine::render() {
    SDL_FillRect(this->screen, &this->screen->clip_rect, SDL_MapRGB(this->screen->format, 0x00, 0x00, 0x00));
    
    
    
    SDL_Flip(this->screen);
    
    SDL_Delay(1000 / 60);
}
