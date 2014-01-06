#include "GameMapObject.h"

const char LuaGameMapObject::className[] = "GameMapObject";

Lunar<LuaGameMapObject>::RegType LuaGameMapObject::methods[] = {
    {"getX", &LuaGameMapObject::getX},
    {"getY", &LuaGameMapObject::getY},
    {"setPosition", &LuaGameMapObject::setPosition},
    {"getProperty", &LuaGameMapObject::getProperty},
    {"setProperty", &LuaGameMapObject::setProperty},
    {0, 0}
};

GameMapObject::GameMapObject() {

}

GameMapObject::~GameMapObject() {

}

SDL_Rect GameMapObject::GetPosition() {
    return SDL_Rect {this->x, this->y, 1, 1};
}

void GameMapObject::SetPosition(int x, int y) {
    this->x = x;

    this->y = y;
}
