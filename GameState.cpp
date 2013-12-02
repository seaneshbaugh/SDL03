#include "GameState.h"

GameState::GameState(SDL_Renderer* renderer) {
    this->renderer = renderer;

    this->pop = false;
}

GameState::~GameState() {

}

GameState* GameState::Update(SDL_Event* event) {
    if (event) {
        this->ProcessInput(event);
    }

    if (this->pop) {
        return NULL;
    } else {
        return this;
    }
}

void GameState::ProcessInput(SDL_Event* event) {

}

void GameState::RenderObjects(std::map <std::string, GameTexture*> textures, std::map <std::string, GameFont*> fonts, std::map <std::string, GameSound*> sounds) {
    
}
