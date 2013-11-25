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

bool Engine::Setup() {
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

void Engine::Start() {
    IntroState* introState = new IntroState();

    this->states.push_back(introState);

    this->MainLoop();
}

void Engine::MainLoop() {
    SDL_Event event;

    bool quit = false;

    // The way this works right now is very tentative and is probably very bad.
    // I have no idea what I'm doing, but I think this should work while things are
    // still simple.
    // The basic idea is, each pass of the loop checks to see if there is a state object
    // left in the stack (I'm using a std::vector like a stack because I'm lazy), if not
    // then do nothing and then quit. Otherwise, check for any pending events then call
    // the update the current state (and quit if desired).
    // This is the tricky part that I'm not convinced is a good idea: if the Update
    // function returns NULL then we pop the current state off the end of the stack. If
    // it returns itself we do nothing. And finally, if it returns a pointer to another
    // GameState object then that state is pushed onto the stack and is now the current
    // state.
    // Without passing the whole Engine object to the update function I can't really
    // figure out a better way of telling the Engine to do stuff with its state stack.
    while (quit == false) {
        if (this->states.size() > 0) {
            int pendingEvent;

            pendingEvent = SDL_PollEvent(&event);

            GameState* currentState = this->states.back();

            GameState* nextState = currentState;

            if (pendingEvent) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }

                nextState = currentState->Update(&event);
            } else {
                nextState = currentState->Update(NULL);
            }

            this->Render();

            if (nextState == NULL) {
                this->states.pop_back();
            } else {
                if (nextState != currentState) {
                    this->states.push_back(nextState);
                }
            }
        } else {
            quit = true;
        }
    }
}

void Engine::Render() {
    SDL_FillRect(this->screen, &this->screen->clip_rect, SDL_MapRGB(this->screen->format, 0x00, 0x00, 0x00));

    this->states.back()->RenderObjects();
    
    SDL_Flip(this->screen);
    
    SDL_Delay(1000 / 60);
}