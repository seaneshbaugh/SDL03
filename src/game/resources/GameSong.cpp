#include "GameSong.h"

GameSong::GameSong() {
    this->music = nullptr;

    this->filename = "";

    this->logger = new Log::Logger("assets.songs");
}

GameSong::GameSong(std::string filename) : GameSong() {
    this->Load(filename);
}

GameSong::~GameSong() {
    this->DestroyMusic();
}

bool GameSong::Load(std::string filename) {
    this->DestroyMusic();

    this->filename = filename;

    this->music = Mix_LoadMUS(filename.c_str());

    if (this->music == nullptr) {
        this->logger->error() << "Error creating music from \"" << filename << "\":" << Mix_GetError();

        return false;
    }

    return true;
}
bool GameSong::DestroyMusic() {
    if (this->music == nullptr) {
        return false;
    }

    this->logger->info() << "Destroying music \"" << this->filename << "\".";

    Mix_FreeMusic(this->music);

    this->logger->info() << "Destroyed music \"" << this->filename << "\".";

    this->music = nullptr;

    return true;
}

