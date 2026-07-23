#ifndef SDL03_Game_Scenes_AnimationPlayer
#define SDL03_Game_Scenes_AnimationPlayer

#include "../graphics/animation_clip.hpp"
#include "../services/locator.hpp"

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

            bool ConsumeCompletedLoop();

        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            std::shared_ptr<Graphics::AnimationClip> currentAnimationClip;
            std::shared_ptr<Graphics::AnimationClip> desiredAnimationClip;
            std::shared_ptr<Graphics::Animation> currentAnimation;
            Direction direction;
            std::vector<Graphics::AnimationFrame>::size_type animationFrame;
            float timeSinceLastAnimationFrame;
            int completedLoops;
        };
    }
}

#endif
