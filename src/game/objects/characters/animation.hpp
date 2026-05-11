#ifndef SDL03_Game_Objects_Characters_Animation
#define SDL03_Game_Objects_Characters_Animation

#include <vector>
#include <iterator>

#include "animation_frame.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            class Animation {
            public:
                Animation(const unsigned int width, const unsigned int height, const std::vector<AnimationFrame> frames);

                std::vector<AnimationFrame> frames;
                unsigned int width;
                unsigned int height;
            };
        }
    }
}

#endif
