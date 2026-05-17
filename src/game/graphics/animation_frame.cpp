#include "animation_frame.hpp"

namespace Game {
    namespace Graphics {
        AnimationFrame::AnimationFrame(const unsigned int offsetX, const unsigned int offsetY) {
            this->offsetX = offsetX;
            this->offsetY = offsetY;
        }
    }
}
