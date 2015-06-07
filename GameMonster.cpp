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

void GameMonster::Render(int x, int y) {
    int w, h;

    SDL_QueryTexture(this->sprite->texture, nullptr, nullptr, &w, &h);

    SDL_Rect position = {x, y, w, h};

    SDL_RenderCopy(this->renderer, this->sprite->texture, nullptr, &position);
}
