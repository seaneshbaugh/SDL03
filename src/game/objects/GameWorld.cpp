#include "GameWorld.h"

GameWorld::GameWorld() {
    this->playerParty = nullptr;

    this->SetStartingPlayerParty();

    this->enemyParty = nullptr;

    this->currentMap = nullptr;

    this->LoadMap("resources/maps/world01.json");
}

GameWorld::~GameWorld() {
    this->UnloadPlayerParty();

    this->UnloadEnemyParty();

    this->UnloadMap();
}

// TODO: Maybe make this be a static method for GameParty.
bool GameWorld::SetStartingPlayerParty() {
    this->UnloadPlayerParty();

    // TODO: Make this be determined by a resource file.
    this->playerParty = new GameParty();

    GameCharacter* sean = new GameCharacter();

    sean->Load("resources/characters/character01.json");

    this->playerParty->characters.push_back(sean);

    GameCharacter* casie = new GameCharacter();

    casie->Load("resources/characters/character02.json");

    this->playerParty->characters.push_back(casie);

    return true;
}

bool GameWorld::UnloadPlayerParty() {
    if (this->playerParty == nullptr) {
        return false;
    }

    delete this->playerParty;

    return true;
}

bool GameWorld::SetEnemyParty(GameMapEncounterArea* encounterArea) {
    this->UnloadEnemyParty();

    this->enemyParty = new GameParty();

    // TODO: Consider making rd and mt be members of GameWorld.
    std::random_device rd;

    std::mt19937_64 mt(rd());

    const int ml = static_cast<int>(encounterArea->mobs.size()) - 1;

    std::uniform_int_distribution<int> dist(0, ml);

    int mobIndex = dist(mt);

    std::vector<std::string> mob = encounterArea->mobs[mobIndex];

    for (std::vector<std::string>::iterator it = mob.begin(); it != mob.end(); it++) {
        GameMonster* monster = new GameMonster();

        // TODO: THIS IS BAD. Figure out a better way. The best solution may be some sort of
        // JSON resource file to act as an index of all monsters where the key is some sort
        // of ID and the value is the file to load it from. While I'm thinking about it,
        // the monster resource files should also include a reference to an AI (LOL) script
        // that the monster will use for determining its actions.
        if (*it == "Slime") {
            monster->Load("resources/monsters/slime.json");
        } else {
            // For now raise an exception since the above needs to be done and there are no
            // other monster types defined yet anyways. If we get here something has gone
            // wrong.
            throw;
        }

        this->enemyParty->characters.push_back(monster);
    }

    return true;
}

bool GameWorld::UnloadEnemyParty() {
    if (this->enemyParty == nullptr) {
        return false;
    }

    delete this->enemyParty;

    return true;
}

bool GameWorld::LoadMap(std::string filename) {
    this->UnloadMap();

    this->currentMap = new GameMap(filename);

    return true;
}

bool GameWorld::UnloadMap() {
    if (this->currentMap == nullptr) {
        return false;
    }

    delete this->currentMap;

    return true;
}
