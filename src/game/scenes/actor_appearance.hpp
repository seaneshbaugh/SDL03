#ifndef SDL03_Game_Scenes_ActorAppearance
#define SDL03_Game_Scenes_ActorAppearance

#include "../graphics/animation.hpp"
#include "../graphics/spritesheet.hpp"
#include "../services/locator.hpp"

namespace Game {
    namespace Scenes {
        class Camera;

        class ActorAppearance {
        public:
            std::shared_ptr<Graphics::Spritesheet> spritesheet;

            ActorAppearance(std::shared_ptr<Graphics::Spritesheet> spritesheet);

            void Render(std::shared_ptr<Graphics::Animation> animation, const size_t frameIndex, const float worldX, const float worldY, std::shared_ptr<Camera> camera) const;
        };
    }
}

#endif
