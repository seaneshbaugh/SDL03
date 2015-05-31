#include "GameInputMapper.h"

// Initialize with the default options.
GameInputMapper::GameInputMapper() {
    this->inputMap[SDLK_UP] = UP_KEY;

    this->inputMap[SDLK_DOWN] = DOWN_KEY;

    this->inputMap[SDLK_LEFT] = LEFT_KEY;

    this->inputMap[SDLK_RIGHT] = RIGHT_KEY;

    this->inputMap[SDLK_z] = CONFIRM_KEY;

    this->inputMap[SDLK_x] = CANCEL_KEY;

    this->inputMap[SDLK_TAB] = MENU_KEY;
}

GameInputMapper::~GameInputMapper() {

}

// This probably needs some sort of error checking to make sure rawKeyValue is an actual SDL key.
int GameInputMapper::SetInputMapKey(int rawKeyValue, int inputValue) {
    for (std::map<int, int>::iterator it = this->inputMap.begin(); it != this->inputMap.end(); it++) {
        if (it->second == inputValue) {
            inputMap.erase(it);
        }
    }

    this->inputMap[rawKeyValue] = inputValue;

    return inputValue;
}

int GameInputMapper::GetInputMapKey(SDL_Event* event) {
    try {
        if (event->type == SDL_KEYDOWN) {
            return this->inputMap.at(event->key.keysym.sym);
        } else {
            return NO_KEY;
        }
    } catch (const std::out_of_range& exception) {
        return NO_KEY;
    }
}
