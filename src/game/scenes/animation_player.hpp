#ifndef SDL03_Game_Scenes_AnimationPlayer
#define SDL03_Game_Scenes_AnimationPlayer

#include <string>

namespace Game {
    namespace Scenes {
        struct AnimationState {

        };

        class AnimationPlayer {
        public:
            AnimationPlayer();
            ~AnimationPlayer();
            void Play(const std::string& animationName);
            void Update(float deltaTime);

        private:
            std::string currentAnimation;
            unsigned int animationFrame;
            float timeSinceLastAnimationFrame;
        };
    }
}

#endif
