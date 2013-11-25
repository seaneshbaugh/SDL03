#include "IntroState.h"

IntroState::IntroState() {
    pop = false;
}

IntroState::~IntroState() {
    
}

GameState* IntroState::Update(SDL_Event* event) {
    if (event) {
        if (event->type == SDL_KEYDOWN || event->type == SDL_MOUSEBUTTONDOWN) {
            this->ProcessInput(event);
        }
    }

    if (this->pop) {
        return NULL;
    } else {
        return this;
    }
}

void IntroState::ProcessInput(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        this->pop = true;
    }
}

void IntroState::RenderObjects() {

}
