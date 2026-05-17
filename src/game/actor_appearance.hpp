#ifndef SDL03_Game_ActorAppearance
#define SDL03_Game_ActorAppearance

#include "services/locator.hpp"
#include "graphics/animation.hpp"

namespace Game {
    class ActorAppearance {
    public:
        ActorAppearance(const std::string& spritesheetName, const unsigned int width, const unsigned int height, const unsigned int offsetX, const unsigned int offsetY);

    private:
        std::string spritesheetName;
        std::shared_ptr<Assets::Texture> spritesheet;
        std::map<std::string, Graphics::Animation> animations;
    };
}

#endif
