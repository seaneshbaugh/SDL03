#include "animation_clip.hpp"

namespace Game {
    namespace Graphics {
        AnimationClip::AnimationClip(const std::map<Scenes::Actor::Direction, Animation>& variants) {
            std::copy(variants.begin(), variants.end(), std::inserter(this->variants, this->variants.begin()));
        }
    }
}
