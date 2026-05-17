#include "animation.hpp"

namespace Game {
    namespace Graphics {
        Animation::Animation(const unsigned int width, const unsigned int height, const std::vector<AnimationFrame> frames) {
            this->width = width;
            this->height = height;
            std::copy(frames.begin(), frames.end(), std::back_inserter(this->frames));
        }
    }
}
