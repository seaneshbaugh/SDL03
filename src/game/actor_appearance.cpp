#include "actor_appearance.hpp"

namespace Game {
    ActorAppearance::ActorAppearance(const std::string& spriteName, const unsigned int width, const unsigned int height, const unsigned int offsetX, const unsigned int offsetY) {
        this->spritesheetName = spriteName;
        this->spritesheet = Services::Locator::TextureService()->GetTexture(spriteName);
    }
}
