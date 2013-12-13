#include "GameTexture.h"

SDL_Renderer* GameTexture::renderer;

GameTexture::GameTexture() {
    this->texture = NULL;

    this->filename = "";
}

GameTexture::GameTexture(std::string filename) {
    this->texture = NULL;

    this->filename = "";

    this->Load(filename);
}

GameTexture::~GameTexture() {
    if (this->texture) {
        SDL_DestroyTexture(this->texture);
    }
}

bool GameTexture::Load(std::string filename) {
    if (this->texture) {
        SDL_DestroyTexture(this->texture);

        this->texture = NULL;
    }

    this->filename = filename;

    SDL_Surface* surface = IMG_Load(filename.c_str());

    if (surface) {
        this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);

        SDL_FreeSurface(surface);

        if (this->texture) {
            return true;
        } else {
            return false;
        }
    } else {
        std::cerr << IMG_GetError() << std::endl;

        return false;
    }
}
