#ifndef SDL03_Game_Graphics_AnimationClip
#define SDL03_Game_Graphics_AnimationClip

#include <map>
#include <memory>

#include "../types.hpp"
#include "animation.hpp"

namespace Game {
    namespace Graphics {
        class AnimationClip {
        public:
            AnimationClip(const std::map<Direction, std::shared_ptr<Animation>>& variants);
            std::shared_ptr<Animation> GetAnimation(const Direction direction) const;
            int GetFramesPerSecond() const;

        private:
            static int defaultFramesPerSecond;

            std::map<Direction, std::shared_ptr<Animation>> variants;
            int framesPerSecond;
        };
    }
}

#endif
