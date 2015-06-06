#include "GameMonster.h"

const char LuaGameMonster::className[] = "GameMonster";

Lunar<LuaGameMonster>::RegType LuaGameMonster::methods[] = {
    {"getName", &LuaGameMonster::getName},
    {"setName", &LuaGameMonster::setName},
    {"getLevel", &LuaGameMonster::getLevel},
    {"setLevel", &LuaGameMonster::setLevel},
    {"getCurrentHitPoints", &LuaGameMonster::getCurrentHitPoints},
    {"setCurrentHitPoints", &LuaGameMonster::setCurrentHitPoints},
    {"getMaxHitPoints", &LuaGameMonster::getMaxHitPoints},
    {"setMaxHitPoints", &LuaGameMonster::setMaxHitPoints},
    {"getCurrentMagicPoints", &LuaGameMonster::getCurrentMagicPoints},
    {"setCurrentMagicPoints", &LuaGameMonster::setCurrentMagicPoints},
    {"getMaxMagicPoints", &LuaGameMonster::getMaxMagicPoints},
    {"setMaxMagicPoints", &LuaGameMonster::setMaxMagicPoints},
    {"damage", &LuaGameMonster::damage},
    {"render", &LuaGameMonster::render},
    {0, 0}
};

GameMonster::GameMonster() {
    this->name = "";

    this->level = 1;

    this->currentHitPoints = 1;

    this->maxHitPoints = 1;

    this->currentMagicPoints = 0;

    this->maxMagicPoints = 0;
}

GameMonster::~GameMonster() {
}

bool GameMonster::ParseMonsterFile(std::string jsonString) {
    JSONNode monsterNode = libjson::parse(jsonString);

    JSONNode::const_iterator i = monsterNode.begin();

    while (i != monsterNode.end()) {
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

bool GameMonster::Load(std::string filename) {
    std::string jsonString;

    if (!FileSystemHelpers::ReadFile(filename, jsonString)) {
        return false;
    }

    if (!this->ParseMonsterFile(jsonString)) {
        return false;
    }

    return true;
}

void GameMonster::Render(int x, int y) {
    int w, h;

    SDL_QueryTexture(this->sprite->texture, nullptr, nullptr, &w, &h);

    SDL_Rect position = {x, y, w, h};

    SDL_RenderCopy(this->renderer, this->sprite->texture, nullptr, &position);
}
