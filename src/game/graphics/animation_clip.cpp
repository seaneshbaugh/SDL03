#include "animation_clip.hpp"

namespace Game {
    namespace Graphics {
        int AnimationClip::defaultFramesPerSecond = 8;

        AnimationClip::AnimationClip(const std::map<Direction, std::shared_ptr<Animation>>& variants) : framesPerSecond(AnimationClip::defaultFramesPerSecond) {
            std::copy(variants.begin(), variants.end(), std::inserter(this->variants, this->variants.begin()));
        }

        std::shared_ptr<Animation> AnimationClip::GetAnimation(const Direction direction) const {
            if (this->variants.empty()) {
                return nullptr;
            }

            if (this->variants.contains(direction)) {
                return this->variants.at(direction);
            }

            return this->variants.begin()->second;
        }

        int AnimationClip::GetFramesPerSecond() const {
            return this->framesPerSecond;
        }
    }
}
