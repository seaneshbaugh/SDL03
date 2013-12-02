#include "GameFont.h"

const int DEFAULT_POINT_SIZE = 16;

GameFont::GameFont() {
    this->font = NULL;

    this->pointSize = DEFAULT_POINT_SIZE;
}

GameFont::~GameFont() {
    if (this->font) {
        TTF_CloseFont(this->font);
    }
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

    this->font = TTF_OpenFont(filename.c_str(), this->pointSize);

    if (this->font != NULL) {
        return true;
    } else {
        return false;
    }
}