#include "animation_player.hpp"

namespace Game {
    namespace Scenes {
        const std::string AnimationPlayer::logChannel = "scene.animation_player";

        AnimationPlayer::AnimationPlayer() : currentAnimationClip(nullptr), desiredAnimationClip(nullptr), currentAnimation(nullptr), direction(Direction::Down), animationFrame(0), timeSinceLastAnimationFrame(0.0f), completedLoops(0) {
            this->logger = Services::Locator::LoggerService()->GetLogger(AnimationPlayer::logChannel);
        }

        AnimationPlayer::~AnimationPlayer() {
        }

        void AnimationPlayer::Play(std::shared_ptr<Graphics::AnimationClip> animationClip) {
            if (this->desiredAnimationClip == animationClip) {
                return;
            }

            if (this->currentAnimationClip == nullptr) {
                this->currentAnimationClip = animationClip;
                this->currentAnimation = this->currentAnimationClip->GetAnimation(this->direction);
                this->animationFrame = 0;
                this->timeSinceLastAnimationFrame = 0.0f;
                this->completedLoops = 0;

                return;
            }

            this->desiredAnimationClip = animationClip;
        }

        void AnimationPlayer::Update(float deltaTime) {
            if (this->desiredAnimationClip != nullptr && this->desiredAnimationClip != this->currentAnimationClip) {
                this->currentAnimationClip = this->desiredAnimationClip;
                this->desiredAnimationClip = nullptr;
                this->animationFrame = 0;
                this->timeSinceLastAnimationFrame = 0.0f;
                this->completedLoops = 0;
                this->currentAnimation = this->currentAnimationClip->GetAnimation(this->direction);

                return;
            }

            if (this->currentAnimationClip == nullptr) {
                return;
            }

            this->timeSinceLastAnimationFrame += deltaTime;

            if (this->timeSinceLastAnimationFrame >= 1.0 / static_cast<float>(this->currentAnimationClip->GetFramesPerSecond())) {
                this->animationFrame = (this->animationFrame + 1) % this->currentAnimation->frames.size();

                if (this->animationFrame == 0) {
                    this->completedLoops++;
                }

                this->timeSinceLastAnimationFrame = 0.0f;
            }
        }

        std::shared_ptr<Graphics::AnimationClip> AnimationPlayer::GetCurrentAnimationClip() const {
            return this->currentAnimationClip;
        }

        std::shared_ptr<Graphics::Animation> AnimationPlayer::GetCurrentAnimation() const {
            if (this->currentAnimationClip == nullptr) {
                return nullptr;
            }

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

        bool AnimationPlayer::ConsumeCompletedLoop() {
            if (this->completedLoops > 0) {
                this->completedLoops--;

                return true;
            }

            return false;
        }
    }
}
