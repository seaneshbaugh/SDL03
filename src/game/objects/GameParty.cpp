#include "GameParty.h"

GameParty::GameParty() {
}

GameParty::~GameParty() {
    for (std::vector<GameCharacter*>::iterator character = this->characters.begin(); character != this->characters.end(); character++) {
        delete (*character);
    }
}
