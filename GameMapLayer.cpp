#include "GameMapLayer.h"

GameMapLayer::GameMapLayer() {
    this->name = "";

    this->visible = true;
}

GameMapLayer::~GameMapLayer() {
    for (std::vector<GameMapObject*>::iterator object = this->objects.begin(); object != this->objects.end(); object++) {
        delete (*object);
    }
}

void GameMapLayer::Render(int xOffset, int yOffset) {

}
