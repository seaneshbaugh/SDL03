#ifndef SDL03_Game_Graphics_Animation
#define SDL03_Game_Graphics_Animation

#include <vector>
#include <iterator>

#include <SDL3/SDL.h>

#include "animation_frame.hpp"

namespace Game {
    namespace Graphics {
        class Animation {
        public:
            int width;
            int height;
            std::vector<AnimationFrame> frames;

            Animation(const int width, const int height, const std::vector<AnimationFrame>& frames);
            SDL_Rect GetFrameRect(const std::vector<AnimationFrame>::size_type frameIndex) const;
        };
    }
}

#endif
