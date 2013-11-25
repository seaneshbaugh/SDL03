#include "Engine.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Engine::Engine() {
    this->screen = NULL;

    this->renderer = NULL;
    
    this->screenWidth = SCREEN_WIDTH;
    
    this->screenHeight = SCREEN_WIDTH;

    this->windowTitle = "Hello, world!";
}

Engine::~Engine() {
    SDL_DestroyWindow(this->screen);

    SDL_Quit();
}

bool Engine::Setup() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return false;
    }

    SDL_CreateWindowAndRenderer(this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL, &this->screen, &this->renderer);

    if (this->screen == NULL || this->renderer == NULL) {
        return false;
    }

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
    // the update function for the current state (and quit if desired).
    // This is the tricky part that I'm not convinced is a good idea: if the Update
    // function returns NULL then we pop the current state off the end of the stack. If
    // it returns itself we do nothing. And finally, if it returns a pointer to another
    // GameState object then that state is pushed onto the stack and is now the current
    // state.
    // Without passing the whole Engine object to the update function I can't really
    // figure out a better way of telling the Engine to do stuff with its state stack.
    // I'm hoping that doing things this way will ensure that only the current state
    // initiates transitions to new states and by having that transition be opaque to
    // the loop the loop itself should remain very simple and unaware of what each
    // state actually means. It should also allow each state to manage its own internal
    // objects independently.
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
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);

    SDL_RenderClear(this->renderer);

    this->states.back()->RenderObjects();

    SDL_RenderPresent(this->renderer);

    SDL_Delay(1000 / 60);
}