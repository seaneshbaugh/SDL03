#include "GameEngine.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FRAMES_PER_SECOND = 60;
const std::string APPLICATION_NAME = "SDL03";

GameEngine::GameEngine() {
    this->screen = nullptr;

    this->renderer = nullptr;
    
    this->screenWidth = SCREEN_WIDTH;
    
    this->screenHeight = SCREEN_HEIGHT;

    this->windowTitle = "Hello, world!";
}

GameEngine::~GameEngine() {
    for (std::vector <GameState*>::iterator it = this->states.begin(); it != this->states.end(); it++) {
        if (*it) {
            delete *it;
        }
    }

    if (this->screen) {
        SDL_DestroyWindow(this->screen);
    }

    Mix_CloseAudio();

    TTF_Quit();

    IMG_Quit();

    SDL_Quit();
}

bool GameEngine::Setup() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return false;
    }

    SDL_CreateWindowAndRenderer(this->screenWidth, this->screenHeight, SDL_WINDOW_OPENGL, &this->screen, &this->renderer);

    if (this->screen == nullptr || this->renderer == nullptr) {
        return false;
    }

    SDL_SetWindowTitle(this->screen, this->windowTitle.c_str());

    GameState::renderer = this->renderer;

    GameTexture::renderer = this->renderer;

    GameText::renderer = this->renderer;

    GameImage::renderer = this->renderer;

    GameMap::renderer = this->renderer;

    if ((IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG) != IMG_INIT_PNG) {
        return false;
    }

    if (TTF_Init() == -1) {
        return false;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        return false;
    }

    this->inputMapper = GameInputMapper();

    GameState::inputMapper = &this->inputMapper;

    return true;
}

void GameEngine::Start() {
    IntroState* introState = new IntroState(nullptr);

    this->states.push_back(introState);

    this->MainLoop();
}

void GameEngine::MainLoop() {
    SDL_Event event;

    bool quit = false;

    // The way this works right now is very tentative and is probably very bad. I have
    // no idea what I'm doing, but I think this should work while things are still
    // simple.
    // The basic idea is, each pass of the loop checks to see if there is a state
    // object left in the stack (I'm using a std::vector like a stack because I'm
    // lazy), if not then do nothing and then quit. Otherwise, check for any pending
    // events then call the update function for the current state (and quit if
    // desired).
    // This is the tricky part that I'm not convinced is a good idea: if the Update
    // function returns nullptr then we pop the current state off the end of the stack.
    // If it returns itself we do nothing. And finally, if it returns a pointer to
    // another GameState object then that state is pushed onto the stack and is now the
    // current state.
    // Without passing the whole GameEngine object to the update function I can't
    // really figure out a better way of telling the GameEngine to do stuff with its
    // state stack.
    // I'm hoping that doing things this way will ensure that only the current state
    // initiates transitions to new states and by having that transition be opaque to
    // the loop, the loop itself should remain very simple and unaware of what each
    // state actually means. It should also allow each state to manage its own internal
    // objects independently.
    while (!quit) {
        if (this->states.size() > 0) {
            int startTicks = SDL_GetTicks();

            int pendingEvent = SDL_PollEvent(&event);

            GameState* currentState = this->states.back();

            GameState* nextState = currentState;

            if (pendingEvent) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }

                nextState = currentState->Update(&event);
            } else {
                nextState = currentState->Update(nullptr);
            }

            this->Render();

            if (nextState == nullptr) {
                delete currentState;

                this->states.pop_back();
            } else {
                if (nextState != currentState) {
                    this->states.push_back(nextState);
                }
            }

            if ((SDL_GetTicks() - startTicks) < (1000 / FRAMES_PER_SECOND)) {
                SDL_Delay((1000 / FRAMES_PER_SECOND) - (SDL_GetTicks() - startTicks));
            }
        } else {
            quit = true;
        }
    }
}

void GameEngine::Render() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);

    SDL_RenderClear(this->renderer);

    this->states.back()->Render();

    SDL_RenderPresent(this->renderer);
}
