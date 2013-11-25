#include "GameState.h"

GameState::GameState() {
    pop = false;
}

GameState::~GameState() {

}

GameState* GameState::Update(SDL_Event* event) {
    this->ProcessInput(event);

    this->RenderObjects();

    return this;
}

void GameState::ProcessInput(SDL_Event* event) {

}

void GameState::RenderObjects() {
    
}
