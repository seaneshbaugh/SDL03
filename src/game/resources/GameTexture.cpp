#include "GameTexture.h"
#include "GameEngine.h"

GameTexture::GameTexture() {
    this->texture = nullptr;

    this->filename = "";

    this->logger = new Log::Logger("assets.textures");
}

GameTexture::GameTexture(std::string filename) : GameTexture() {
    this->Load(filename);
}

GameTexture::~GameTexture() {
    this->DestroyTexture();
}

bool GameTexture::Load(std::string filename) {
    this->DestroyTexture();

    this->filename = filename;

    SDL_Surface* surface = IMG_Load(this->filename.c_str());

    if (!surface) {
        this->logger->error() << "Error creating surface from \"" << filename << "\":" << IMG_GetError();

        return false;
    }

    this->texture = SDL_CreateTextureFromSurface(Services::Locator::RendererService()->GetRenderer().get(), surface);

    SDL_FreeSurface(surface);

    if (!this->texture) {
        this->logger->error() << "Error creating texture from \"" << filename << "\":" << SDL_GetError();

        return false;
    }

    return true;
}

bool GameTexture::DestroyTexture() {
    if (this->texture == nullptr) {
        return false;
    }

    this->logger->info() << "Destroying texture \"" << this->filename << "\".";

    SDL_DestroyTexture(this->texture);

    this->logger->info() << "Destroyed texture \"" << this->filename << "\".";

    this->texture = nullptr;

    return true;
}
