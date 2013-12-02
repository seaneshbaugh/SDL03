#include "IntroState.h"

IntroState::IntroState(SDL_Renderer* renderer) : GameState(renderer) {
    this->renderer = renderer;

    this->pop = false;
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

// This needs to be completely redone. Possibly by having each state have its own
// lists of resources that are loaded when transitioning to the state. This should
// avoid having to pass in everything awkwardly. It would however greatly increase
// the likelyhood of needing to reload resouces multiple times, especially textures
// and fonts.
void IntroState::RenderObjects(std::map <std::string, GameTexture*> textures, std::map <std::string, GameFont*> fonts, std::map <std::string, GameSound*> sounds) {
    SDL_Texture *textTexture = NULL;

    SDL_Color color = { 255, 255, 255 };

    SDL_Rect textLocation;

    SDL_Surface *textSurface = TTF_RenderText_Blended(fonts["DroidSans"]->font, "Hello, world!", color);

    textTexture = SDL_CreateTextureFromSurface(this->renderer, textSurface);

    textLocation.h = textSurface->h;

    textLocation.w = textSurface->w;

    textLocation.x = 100;

    textLocation.y = 100;

    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(this->renderer, textTexture, NULL, &textLocation);

    // Need to figure out how to store textures as objects or something since this is
    // creating a new texture on every pass which is very inefficient.
    SDL_DestroyTexture(textTexture);
}
