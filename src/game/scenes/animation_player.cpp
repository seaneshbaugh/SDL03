#include "animation_player.hpp"

namespace Game {
    namespace Scenes {
        AnimationPlayer::AnimationPlayer() : currentAnimationClip(nullptr), direction(Direction::Down), animationFrame(0), timeSinceLastAnimationFrame(0.0f) {
        }

        AnimationPlayer::~AnimationPlayer() {
        }

        void AnimationPlayer::Play(std::shared_ptr<Graphics::AnimationClip> animationClip) {
            if (this->currentAnimationClip == animationClip) {
                return;
            }

            this->currentAnimationClip = animationClip;
            this->animationFrame = 0;
            this->timeSinceLastAnimationFrame = 0.0f;
        }

        void AnimationPlayer::Update(float deltaTime) {
            if (this->currentAnimationClip == nullptr || this->currentAnimationClip->GetAnimation(this->direction) == nullptr) {
                return;
            }

            this->timeSinceLastAnimationFrame += deltaTime;

            if (this->timeSinceLastAnimationFrame >= 0.125f) {
                this->animationFrame = (this->animationFrame + 1) % this->currentAnimationClip->GetAnimation(this->direction)->frames.size();
                this->timeSinceLastAnimationFrame = 0.0f;
            }
        }

        std::shared_ptr<Graphics::Animation> AnimationPlayer::GetCurrentAnimation() const {
            return this->currentAnimationClip->GetAnimation(this->direction);
        }

        Direction AnimationPlayer::GetDirection() const {
           return this->direction;
        }

        std::vector<Graphics::AnimationFrame>::size_type AnimationPlayer::GetCurrentAnimationFrame() const {
            return this->animationFrame;
        }

        void AnimationPlayer::SetDirection(const Direction direction) {
            this->direction = direction;
        }
    }
}
