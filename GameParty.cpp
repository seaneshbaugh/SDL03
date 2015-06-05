#include "GameParty.h"

GameParty::GameParty() {
}

GameParty::~GameParty() {
}

GameParty* GameParty::operator<<(GameCharacter newCharacter) {
    this->party.push_back(newCharacter);

    return this;
}