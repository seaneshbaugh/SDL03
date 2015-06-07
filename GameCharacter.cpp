#include "GameCharacter.h"

const unsigned long long MAX_LEVEL = 99;
const unsigned long long MAX_MAX_HIT_POINTS = 999999999999999;
const unsigned long long MAX_MAX_MAGIC_POINTS = 999999999999999;

SDL_Renderer* GameCharacter::renderer;

const char LuaGameCharacter::className[] = "GameCharacter";

Lunar<LuaGameCharacter>::RegType LuaGameCharacter::methods[] = {
    {"getName", &LuaGameCharacter::getName},
    {"setName", &LuaGameCharacter::setName},
    {"getLevel", &LuaGameCharacter::getLevel},
    {"setLevel", &LuaGameCharacter::setLevel},
    {"getCurrentHitPoints", &LuaGameCharacter::getCurrentHitPoints},
    {"setCurrentHitPoints", &LuaGameCharacter::setCurrentHitPoints},
    {"getMaxHitPoints", &LuaGameCharacter::getMaxHitPoints},
    {"setMaxHitPoints", &LuaGameCharacter::setMaxHitPoints},
    {"getCurrentMagicPoints", &LuaGameCharacter::getCurrentMagicPoints},
    {"setCurrentMagicPoints", &LuaGameCharacter::setCurrentMagicPoints},
    {"getMaxMagicPoints", &LuaGameCharacter::getMaxMagicPoints},
    {"setMaxMagicPoints", &LuaGameCharacter::setMaxMagicPoints},
    {"damage", &LuaGameCharacter::damage},
    {"render", &LuaGameCharacter::render},
    {0, 0}
};

GameCharacter::GameCharacter() {
    this->name = "";

    this->level = 1;

    this->currentHitPoints = 1;

    this->maxHitPoints = 1;

    this->currentMagicPoints = 0;

    this->maxMagicPoints = 0;

    this->sprite = nullptr;
}

GameCharacter::~GameCharacter() {
    if (this->sprite) {
        delete this->sprite;
    }
}

unsigned long long GameCharacter::GetLevel() {
    return this->level;
}

unsigned long long GameCharacter::SetLevel(unsigned long long newLevel) {
    if (newLevel < 1) {
        newLevel = 1;
    } else {
        if (newLevel > MAX_LEVEL) {
            newLevel = MAX_LEVEL;
        }
    }

    this->level = newLevel;

    return this->level;
}

unsigned long long GameCharacter::GetCurrentHitPoints() {
    return this->currentHitPoints;
}

unsigned long long GameCharacter::SetCurrentHitPoints(unsigned long long newCurrentHitPoints) {
    if (newCurrentHitPoints > this->maxHitPoints) {
        newCurrentHitPoints = this->maxHitPoints;
    }

    this->currentHitPoints = newCurrentHitPoints;

    return this->currentHitPoints;
}

unsigned long long GameCharacter::GetMaxHitPoints() {
    return this->maxHitPoints;
}

unsigned long long GameCharacter::SetMaxHitPoints(unsigned long long newMaxHitPoints) {
    if (newMaxHitPoints < 1) {
        newMaxHitPoints = 1;
    } else {
        if (newMaxHitPoints > MAX_MAX_HIT_POINTS) {
            newMaxHitPoints = MAX_MAX_HIT_POINTS;
        }
    }

    this->maxHitPoints = newMaxHitPoints;

    if (this->currentHitPoints > this->maxHitPoints) {
        this->currentHitPoints = this->maxHitPoints;
    }

    return this->maxHitPoints;
}

unsigned long long GameCharacter::GetCurrentMagicPoints() {
    return this->currentMagicPoints;
}

unsigned long long GameCharacter::SetCurrentMagicPoints(unsigned long long newCurrentMagicPoints) {
    if (newCurrentMagicPoints > this->currentMagicPoints) {
        newCurrentMagicPoints = this->currentMagicPoints;
    }

    this->currentMagicPoints = newCurrentMagicPoints;

    return this->currentMagicPoints;
}

unsigned long long GameCharacter::GetMaxMagicPoints() {
    return this->maxMagicPoints;
}

unsigned long long GameCharacter::SetMaxMagicPoints(unsigned long long newMaxMagicPoints) {
    if (newMaxMagicPoints < 1) {
        newMaxMagicPoints = 1;
    } else {
        if (newMaxMagicPoints > MAX_MAX_MAGIC_POINTS) {
            newMaxMagicPoints = MAX_MAX_MAGIC_POINTS;
        }
    }

    this->maxMagicPoints = newMaxMagicPoints;

    if (this->currentMagicPoints > this->maxMagicPoints) {
        this->currentMagicPoints = this->maxMagicPoints;
    }

    return this->maxMagicPoints;
}

unsigned long long GameCharacter::Damage(unsigned long long damage) {
    unsigned long long oldCurrentHitPoints = this->currentHitPoints;

    unsigned long long newCurrentHitPoints = oldCurrentHitPoints - damage;

    if (newCurrentHitPoints > oldCurrentHitPoints) {
        newCurrentHitPoints = 0;
    }

    this->SetCurrentHitPoints(newCurrentHitPoints);

    return oldCurrentHitPoints - this->currentHitPoints;
}

bool GameCharacter::ParseCharacterFile(std::string jsonString) {
    JSONNode characterNode = libjson::parse(jsonString);

    JSONNode::const_iterator i = characterNode.begin();

    while (i != characterNode.end()) {
        if (i->name() == "name" && i->type() == JSON_STRING) {
            this->name = i->as_string();
        } else {
            if (i->name() == "level" && i->type() == JSON_NUMBER) {
                this->SetLevel(static_cast<unsigned long long>(i->as_int()));
            } else {
                if (i->name() == "hitPoints" && i->type() == JSON_NUMBER) {
                    this->SetMaxHitPoints(static_cast<unsigned long long>(i->as_int()));

                    this->SetCurrentHitPoints(static_cast<unsigned long long>(i->as_int()));
                } else {
                    if (i->name() == "magicPoints" && i->type() == JSON_NUMBER) {
                        this->SetMaxMagicPoints(static_cast<unsigned long long>(i->as_int()));

                        this->SetCurrentMagicPoints(static_cast<unsigned long long>(i->as_int()));
                    } else {
                        if (i->name() == "sprite" && i->type() == JSON_STRING) {
                            this->sprite = new GameTexture(i->as_string());
                        }
                    }
                }
            }
        }

        i++;
    }

    return true;
}

bool GameCharacter::Load(std::string filename) {
    std::string jsonString;

    if (!FileSystemHelpers::ReadFile(filename, jsonString)) {
        return false;
    }

    if (!this->ParseCharacterFile(jsonString)) {
        return false;
    }
    
    return true;
}

void GameCharacter::Render(int x, int y) {
    SDL_Rect srcrect = {32, 32, 32, 32};

    // int w, h;

    // SDL_QueryTexture(this->sprite->texture, nullptr, nullptr, &w, &h);

    // Not sure how I'm going to calculate this later.
    SDL_Rect position = {x, y, 32, 32};

    if (this->sprite) {

        SDL_RenderCopy(this->renderer, this->sprite->texture, &srcrect, &position);
    }
}