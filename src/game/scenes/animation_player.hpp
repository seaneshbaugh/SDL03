#ifndef SDL03_Game_Scenes_AnimationPlayer
#define SDL03_Game_Scenes_AnimationPlayer

#include "../graphics/animation_clip.hpp"

namespace Game {
    namespace Scenes {
        class AnimationPlayer {
        public:
            AnimationPlayer();
            ~AnimationPlayer();

            void Play(std::shared_ptr<Graphics::AnimationClip> animationClip);
            void Update(float deltaTime);

            std::shared_ptr<Graphics::Animation> GetCurrentAnimation() const;
            Direction GetDirection() const;
            std::vector<Graphics::AnimationFrame>::size_type GetCurrentAnimationFrame() const;

            void SetDirection(const Direction direction);

        private:
            std::shared_ptr<Graphics::AnimationClip> currentAnimationClip;
            Direction direction;
            std::vector<Graphics::AnimationFrame>::size_type animationFrame;
            float timeSinceLastAnimationFrame;
        };
    }
}

#endif
