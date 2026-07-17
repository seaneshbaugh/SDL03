#include "animation_player.hpp"

namespace Game {
    namespace Scenes {
        AnimationPlayer::AnimationPlayer() {
            this->currentAnimation = "";
            this->animationFrame = 0;
            this->timeSinceLastAnimationFrame = 0.0f;
        }

        AnimationPlayer::~AnimationPlayer() {
        }

        void AnimationPlayer::Play(const std::string& animationName) {
            if (this->currentAnimation != animationName) {
                this->currentAnimation = animationName;
                this->animationFrame = 0;
                this->timeSinceLastAnimationFrame = 0.0f;
            }
        }

        void AnimationPlayer::Update(float deltaTime) {
            if (this->currentAnimation.empty()) {
                return;
            }
            this->timeSinceLastAnimationFrame += deltaTime;
            // Assuming each frame lasts 0.125 seconds (8 frames per second)
            if (this->timeSinceLastAnimationFrame >= 0.125f) {
                this->animationFrame++;
                this->timeSinceLastAnimationFrame = 0.0f;
                // Loop the animation
                // Here you would typically check the number of frames in the current animation
                // For simplicity, let's assume there are 8 frames in every animation
                if (this->animationFrame >= 8) {
                    this->animationFrame = 0;
                }
            }
        }
    }
}
