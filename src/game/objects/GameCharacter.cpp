#include "GameCharacter.h"
#include "GameEngine.h"

const unsigned long long MAX_LEVEL = 99;
const unsigned long long MAX_MAX_HIT_POINTS = 999999999999999;
const unsigned long long MAX_MAX_MAGIC_POINTS = 999999999999999;
const unsigned long long MAX_STAT = 9999;

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
    {"getStrength", &LuaGameCharacter::getStrength},
    {"getDexterity", &LuaGameCharacter::getDexterity},
    {"getIntelligence", &LuaGameCharacter::getIntelligence},
    {"atbStart", &LuaGameCharacter::atbStart},
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

    this->strength = 1;

    this->dexterity = 1;

    this->intelligence = 1;

    this->vitality = 1;

    this->stamina = 1;

    this->luck = 1;

    this->spriteName = "";

    this->spritesheetName = "";
}

GameCharacter::~GameCharacter() {
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

unsigned long long GameCharacter::GetStrength() {
    return this->strength;
}

unsigned long long GameCharacter::SetStrength(unsigned long long newStrength) {
    if (newStrength < 1) {
        newStrength = 1;
    } else {
        if (newStrength > MAX_STAT) {
            newStrength = MAX_STAT;
        }
    }

    this->strength = newStrength;

    return this->strength;
}

unsigned long long GameCharacter::GetDexterity() {
    return this->dexterity;
}

unsigned long long GameCharacter::SetDexterity(unsigned long long newDexterity) {
    if (newDexterity < 1) {
        newDexterity = 1;
    } else {
        if (newDexterity > MAX_STAT) {
            newDexterity = MAX_STAT;
        }
    }

    this->dexterity = newDexterity;

    return this->strength;
}

unsigned long long GameCharacter::GetIntelligence() {
    return this->intelligence;
}

unsigned long long GameCharacter::SetIntelligence(unsigned long long newIntelligence) {
    if (newIntelligence < 1) {
        newIntelligence = 1;
    } else {
        if (newIntelligence > MAX_STAT) {
            newIntelligence = MAX_STAT;
        }
    }

    this->intelligence = newIntelligence;

    return this->intelligence;
}

unsigned long long GameCharacter::GetVitality() {
    return this->vitality;
}

unsigned long long GameCharacter::SetVitality(unsigned long long newVitality) {
    if (newVitality < 1) {
        newVitality = 1;
    } else {
        if (newVitality > MAX_STAT) {
            newVitality = MAX_STAT;
        }
    }

    this->vitality = newVitality;

    return this->vitality;
}

unsigned long long GameCharacter::GetStamina() {
    return this->stamina;
}

unsigned long long GameCharacter::SetStamina(unsigned long long newStamina) {
    if (newStamina < 1) {
        newStamina = 1;
    } else {
        if (newStamina > MAX_STAT) {
            newStamina = MAX_STAT;
        }
    }

    this->stamina = newStamina;

    return this->stamina;
}

unsigned long long GameCharacter::GetLuck() {
    return this->luck;
}

unsigned long long GameCharacter::SetLuck(unsigned long long newLuck) {
    if (newLuck < 1) {
        newLuck = 1;
    } else {
        if (newLuck > MAX_STAT) {
            newLuck = MAX_STAT;
        }
    }

    this->luck = newLuck;

    return this->luck;
}

// I would have put this in the Lua script but I don't know enough about how it does
// arithmetic (it's all floating point?) and even less about how it handles random
// numbers to trust it. I may end up making a collection of functions that handle
// all of the random number generation I will need and have those functions make
// calls to std::uniform_int_distribution and std::uniform_real_distribution so I
// can keep logic like this out of the C++ code.
// As a character's dexterity approaches 255 (which is well below the maximum, but
// still very high as I envision most level 99 characters topping out in the 110 to
// 140 range) their ATB start approaches a minimum value of 100 (i.e. ready to take
// action immediately, as if they had a preemptive attack). In addition to the
// minimum value they also get a bonus equal to a random number between 0 and their
// dexterity over 255. In other words, at 128 dexterity the character will start
// with at least 50  but possibly up to 100. As their minimum start value increases
// so does their possible maximum start value. A start value over 100 makes no sense
// so no matter how dexterous the character is their start value is limited to 100.
int GameCharacter::ATBStart() {
    double dexterityRatio = (static_cast<double>(this->dexterity) / 255.0) * 100.0;

    std::random_device rd;

    std::mt19937 mt(rd());

    std::uniform_real_distribution<double> dist(0.0, dexterityRatio);

    double atbStart = std::floor(dexterityRatio + dist(mt));

    if (atbStart > 100.0) {
        atbStart = 100.0;
    }

    return static_cast<int>(atbStart);
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
    CharacterParser parser = CharacterParser();

    return parser.Parse(jsonString, this);
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
    const SDL_Rect srcrect = {32, 32, 32, 32};

    const SDL_Rect dstrect = {x, y, 32, 32};

    Services::Locator::RendererService()->Render(this->sprite, &srcrect, &dstrect);
}
