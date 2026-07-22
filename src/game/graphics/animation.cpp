#include "animation.hpp"

namespace Game {
    namespace Graphics {
        Animation::Animation(const int width, const int height, const std::vector<AnimationFrame>& frames) : width(width), height(height) {
            std::copy(frames.begin(), frames.end(), std::back_inserter(this->frames));
        }

        SDL_Rect Animation::GetFrameRect(const std::vector<AnimationFrame>::size_type frameIndex) const {
            const AnimationFrame& frame = this->frames.at(frameIndex);

            return SDL_Rect{frame.offsetX, frame.offsetY, this->width, this->height};
        }
    }
}
