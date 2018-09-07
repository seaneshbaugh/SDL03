#include "GameImage.h"
#include "GameEngine.h"

const char LuaGameImage::className[] = "GameImage";

Lunar<LuaGameImage>::RegType LuaGameImage::methods[] = {
    {"getX", &LuaGameImage::getX},
    {"getY", &LuaGameImage::getY},
    {"getWidth", &LuaGameImage::getWidth},
    {"getHeight", &LuaGameImage::getHeight},
    {"setPosition", &LuaGameImage::setPosition},
    {"render", &LuaGameImage::render},
    {"renderWithClip", &LuaGameImage::renderWithClip},
    {0, 0}
};

GameImage::GameImage() {
    this->texture = nullptr;

    this->x = 0;

    this->y = 0;

    this->textureLocation = {0, 0, 0, 0};
}

GameImage::GameImage(GameTexture* texture, int x, int y) : GameImage() {
    this->SetTexture(texture);

    this->SetPosition(x, y);
}

GameImage::~GameImage() {
}

void GameImage::SetTexture(GameTexture* texture) {
    this->texture = texture;

    if (this->texture) {
        SDL_QueryTexture(this->texture->texture, nullptr, nullptr, &this->width, &this->height);
    }
}

SDL_Rect GameImage::GetPosition() {
    return this->textureLocation;
}

void GameImage::SetPosition(int x, int y) {
    this->x = x;

    this->y = y;

    this->textureLocation = {this->x, this->y, this->width, this->height};
}

void GameImage::Render(SDL_Rect* clip) {
    if (this->texture == nullptr) {
        return;
    }

    SDL_Rect renderQuad = {this->x, this->y, this->width, this->height};

    if (clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(GameEngine::currentRenderer, this->texture->texture, clip, &renderQuad);
}
