#ifndef SDL03_Game_Graphics_AnimationClip
#define SDL03_Game_Graphics_AnimationClip

#include <map>

#include "../scenes/actor.hpp"
#include "animation.hpp"

namespace Game {
    namespace Graphics {
        class AnimationClip {
        public:
            std::map<Scenes::Actor::Direction, Animation> variants;

            AnimationClip(const std::map<Scenes::Actor::Direction, Animation>& variants);
        };
    }
}

#endif
