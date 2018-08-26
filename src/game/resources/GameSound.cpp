#include "GameSound.h"

GameSound::GameSound() {
    this->chunk = nullptr;

    this->filename = "";

    this->logger = new Log::Logger("assets.sounds");
}

GameSound::GameSound(std::string filename) : GameSound() {
    this->Load(filename);
}

GameSound::~GameSound() {
    this->DestroyChunk();
}

bool GameSound::Load(std::string filename) {
    this->DestroyChunk();

    this->filename = filename;

    this->chunk = Mix_LoadWAV(this->filename.c_str());

    if (this->chunk == nullptr) {
        this->logger->error() << "Error creating chunk from \"" << filename << "\":" << Mix_GetError();

        return false;
    }

    return true;
}

bool GameSound::DestroyChunk() {
    if (this->chunk == nullptr) {
        return false;
    }

    this->logger->info() << "Destroying chunk \"" << this->filename << "\".";

    Mix_FreeChunk(this->chunk);

    this->logger->info() << "Destroyed chunk \"" << this->filename << "\".";

    this->chunk = nullptr;

    return true;
}
