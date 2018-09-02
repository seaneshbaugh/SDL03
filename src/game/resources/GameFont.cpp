#include "GameFont.h"

const int DEFAULT_POINT_SIZE = 10;

GameFont::GameFont() {
    this->font = nullptr;

    this->filename = "";

    this->pointSize = DEFAULT_POINT_SIZE;

    this->logger = new Log::Logger("assets.fonts");
}

GameFont::GameFont(std::string filename) : GameFont() {
    this->Load(filename);
}

GameFont::GameFont(std::string filename, int pointSize) : GameFont() {
    this->Load(filename, pointSize);
}

GameFont::~GameFont() {
    this->DestroyFont();
}

bool GameFont::Load(std::string filename) {
    return this->Load(filename, this->pointSize);
}

bool GameFont::Load(std::string filename, int pointSize) {
    if (this->font) {
        TTF_CloseFont(this->font);

        this->font = NULL;
    }

    this->filename = filename;

    this->pointSize = pointSize;
    
    this->logger->debug() << "Loading font \"" << filename << "\".";

    this->font = TTF_OpenFont(filename.c_str(), this->pointSize);

    if (this->font != NULL) {
        this->logger->debug() << "Successfully loaded font.";

        return true;
    } else {
        this->logger->error() << "Failed to loaded font.";

        return false;
    }
}

bool GameFont::DestroyFont() {
    if (this->font == nullptr) {
        return false;
    }

    this->logger->info() << "Destroying font \"" << this->filename << "\".";

    TTF_CloseFont(this->font);

    this->logger->info() << "Destroyed font \"" << this->filename << "\".";

    this->font = nullptr;

    return true;
}
