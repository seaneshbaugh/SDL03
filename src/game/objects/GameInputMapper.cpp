#include "GameInputMapper.h"

// Initialize with the default options.
GameInputMapper::GameInputMapper() : logger("input") {
    this->SetDefaultInputMap();
}

GameInputMapper::~GameInputMapper() {
}

std::map<int, int>::size_type GameInputMapper::MapKeys(std::map<int, int> keys) {
    for (std::map<int, int>::iterator key = keys.begin(); key != keys.end(); key++) {
        this->SetInputMapKey(key->first, key->second);
    }

    return keys.size();
}

// This probably needs some sort of error checking to make sure rawKeyValue is an actual SDL key.
int GameInputMapper::SetInputMapKey(int rawKeyValue, int inputValue) {
    for (std::map<int, int>::iterator key = this->inputMap.begin(); key != this->inputMap.end();) {
        if (key->second == inputValue) {
            this->inputMap.erase(key++);
        } else {
            ++key;
        }
    }

    this->inputMap[rawKeyValue] = inputValue;

    return inputValue;
}

int GameInputMapper::GetInputMapKey(const SDL_Event* event) {
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

void GameInputMapper::SetDefaultInputMap() {
    this->inputMap[SDLK_UP] = UP_KEY;

    this->inputMap[SDLK_DOWN] = DOWN_KEY;

    this->inputMap[SDLK_LEFT] = LEFT_KEY;

    this->inputMap[SDLK_RIGHT] = RIGHT_KEY;

    this->inputMap[SDLK_z] = CONFIRM_KEY;

    this->inputMap[SDLK_x] = CANCEL_KEY;

    this->inputMap[SDLK_TAB] = MENU_KEY;
}
