#include "actor_appearance.hpp"
#include "camera.hpp"

namespace Game {
    ActorAppearance::ActorAppearance(std::shared_ptr<Graphics::Spritesheet> spritesheet) {
        this->spritesheet = spritesheet;
    }

    void ActorAppearance::Render(const std::string& animationName, const unsigned int frameIndex, const float worldX, const float worldY, const int tileheight, std::shared_ptr<Camera> camera) {
        const SDL_Rect spriteRect = this->spritesheet->GetSpriteRect(animationName, frameIndex);
        float screenX = worldX - camera->x;
        float screenY = worldY - camera->y - (static_cast<float>(spriteRect.h) - static_cast<float>(tileheight));
        SDL_FRect srcrect = {0.0f, 0.0f, 0.0f, 0.0f};
        SDL_RectToFRect(&spriteRect, &srcrect);
        const SDL_FRect dstrect = {screenX, screenY, srcrect.w, srcrect.h};
        Services::Locator::VideoService()->RenderTexture(this->spritesheet->GetTexture(), &srcrect, &dstrect);
    }
}
