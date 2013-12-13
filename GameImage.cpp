#include "GameImage.h"

SDL_Renderer* GameImage::renderer;

const char LuaGameImage::className[] = "GameImage";

Lunar<LuaGameImage>::RegType LuaGameImage::methods[] = {
    {"getX", &LuaGameImage::getX},
    {"getY", &LuaGameImage::getY},
    {"getWidth", &LuaGameImage::getWidth},
    {"getHeight", &LuaGameImage::getHeight},
    {"setPosition", &LuaGameImage::setPosition},
    {"render", &LuaGameImage::render},
    {0, 0}
};

GameImage::GameImage() {
    this->texture = NULL;

    this->x = 0;

    this->y = 0;

    this->textureLocation = {0, 0, 0, 0};
}

GameImage::GameImage(GameTexture* texture, int x, int y) {
    this->texture = texture;

    this->SetPosition(x, y);
}

GameImage::~GameImage() {

}

SDL_Rect GameImage::GetPosition() {
    return this->textureLocation;
}

void GameImage::SetPosition(int x, int y) {
    this->x = x;

    this->y = y;

    int w = 0;

    int h = 0;

    if (this->texture) {
        SDL_QueryTexture(this->texture->texture, NULL, NULL, &w, &h);
    }

    this->textureLocation = {x, y, w, h};
}

void GameImage::Render() {
    if (this->texture) {
        SDL_RenderCopy(this->renderer, this->texture->texture, NULL, &this->textureLocation);
    }
}
