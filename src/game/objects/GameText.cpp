#include "GameText.h"
#include "GameEngine.h"

const char LuaGameText::className[] = "GameText";

Lunar<LuaGameText>::RegType LuaGameText::methods[] = {
    {"getText", &LuaGameText::getText},
    {"setText", &LuaGameText::setText},
    {"getX", &LuaGameText::getX},
    {"getY", &LuaGameText::getY},
    {"getWidth", &LuaGameText::getWidth},
    {"getHeight", &LuaGameText::getHeight},
    {"setText", &LuaGameText::setText},
    {"setPosition", &LuaGameText::setPosition},
    {"setColor", &LuaGameText::setColor},
    {"render", &LuaGameText::render},
    {0, 0}
};

GameText::GameText() {
    this->text = "";

    this->font = NULL;

    this->x = 0;

    this->y = 0;

    this->color = {255, 255, 255};

    this->textLocation = {0, 0, 0, 0};

    this->texture = NULL;
}

GameText::GameText(std::string text, GameFont* font, int x, int y, SDL_Color color) {
    this->text = text;

    this->font = font;

    this->x = x;

    this->y = y;

    this->color = color;

    this->textLocation = {0, 0, 0, 0};

    this->texture = NULL;

    this->UpdateTexture();
}

GameText::~GameText() {
    if (this->texture) {
        SDL_DestroyTexture(this->texture);
    }
}

std::string GameText::GetText() {
    return this->text;
}

void GameText::SetText(std::string text) {
    this->text = text;

    this->UpdateTexture();
}

void GameText::SetFont(GameFont* font) {
    this->font = font;

    this->UpdateTexture();
}

SDL_Rect GameText::GetPosition() {
    return this->textLocation;
}

void GameText::SetPosition(int x, int y) {
    this->x = x;

    this->y = y;

    int w = 0;

    int h = 0;

    if (this->texture) {
        SDL_QueryTexture(this->texture, NULL, NULL, &w, &h);
    }

    this->textLocation = {x, y, w, h};
}

void GameText::SetColor(SDL_Color color) {
    this->color = color;

    this->UpdateTexture();
}

void GameText::Render() {
    if (this->texture) {
        SDL_RenderCopy(GameEngine::currentRenderer, this->texture, NULL, &this->textLocation);
    }
}

void GameText::UpdateTexture() {
    if (this->texture) {
        SDL_DestroyTexture(this->texture);

        this->texture = NULL;
    }

    SDL_Surface *textSurface = TTF_RenderText_Blended(this->font->font, this->text.c_str(), this->color);

    this->texture = SDL_CreateTextureFromSurface(GameEngine::currentRenderer, textSurface);

    SDL_FreeSurface(textSurface);

    this->SetPosition(this->x, this->y);
}
