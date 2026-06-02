#ifndef SDL03_Game_ActorAppearance
#define SDL03_Game_ActorAppearance

#include "../services/locator.hpp"
#include "../graphics/spritesheet.hpp"

namespace Game {
    namespace Scenes {
        class Camera;

        class ActorAppearance {
        public:
            std::shared_ptr<Graphics::Spritesheet> spritesheet;

            ActorAppearance(std::shared_ptr<Graphics::Spritesheet> spritesheet);
            void Render(const std::string& animationName, const unsigned int frameIndex, const float worldX, const float worldY, std::shared_ptr<Camera> camera);
        };
    }
}

#endif
