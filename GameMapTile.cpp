#include "GameMapTile.h"

GameMapTile::GameMapTile() {
    this->id = 0;

    this->texture = nullptr;
}

GameMapTile::~GameMapTile() {
    if (this->texture) {
        delete this->texture;
    }
}
