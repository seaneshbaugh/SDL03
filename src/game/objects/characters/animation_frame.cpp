#include "animation_frame.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            AnimationFrame::AnimationFrame(const unsigned int offsetX, const unsigned int offsetY) {
                this->offsetX = offsetX;
                this->offsetY = offsetY;
            }
        }
    }
}
