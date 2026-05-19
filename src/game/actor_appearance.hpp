#ifndef SDL03_Game_ActorAppearance
#define SDL03_Game_ActorAppearance

#include "services/locator.hpp"
#include "graphics/spritesheet.hpp"

namespace Game {
    class Camera;

    class ActorAppearance {
    public:
        ActorAppearance(std::shared_ptr<Graphics::Spritesheet> spritesheet);
        void Render(const std::string& animationName, const unsigned int frameIndex, const float worldX, const float worldY, const int tileheight, std::shared_ptr<Camera> camera);

    private:
        std::shared_ptr<Graphics::Spritesheet> spritesheet;
    };
}

#endif
