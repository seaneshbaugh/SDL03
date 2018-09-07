#include "GameFont.h"

const int GameFont::DEFAULT_FONT_SIZE = 10;

GameFont::GameFont() {
    this->font = nullptr;

    this->filename = "";

    this->pointSize = DEFAULT_FONT_SIZE;

    this->logger = new Log::Logger("assets.fonts");
}

GameFont::GameFont(const std::string filename) : GameFont() {
    this->Load(filename);
}

GameFont::GameFont(const std::string filename, const int fontSize) : GameFont() {
    this->logger->debug() << "Loading font \"" << filename << "\" at size " << fontSize << ".";
    this->Load(filename, fontSize);
}

GameFont::~GameFont() {
    this->DestroyFont();
}

bool GameFont::Load(std::string filename) {
    return this->Load(filename, this->pointSize);
}

bool GameFont::Load(std::string filename, int pointSize) {
    if (this->font != nullptr) {
        TTF_CloseFont(this->font);

        this->font = nullptr;
    }

    this->filename = filename;

    this->pointSize = pointSize;
    
    this->logger->debug() << "Loading font \"" << filename << "\".";

    this->font = TTF_OpenFont(filename.c_str(), this->pointSize);

    if (this->font != nullptr) {
        this->logger->debug() << "Successfully loaded font at size " << this->pointSize << ".";

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

    this->logger->debug() << "Destroying font \"" << this->filename << "\".";

    TTF_CloseFont(this->font);

    this->logger->debug() << "Destroyed font \"" << this->filename << "\".";

    this->font = nullptr;

    return true;
}
