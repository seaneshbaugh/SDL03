#include "animate_actor.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                AnimateActor::AnimateActor(Scenes::Scene* scene, const std::string& actorId, const std::string& animationName, float duration) : scene(scene), actorId(actorId), animationName(animationName), duration(duration), elapsedTime(0.0f), started(false), failed(false), previousAnimation(Actor::Animation::Stand), previousAnimationRestored(false) {
                }

                AnimateActor::~AnimateActor() {
                }

                void AnimateActor::Start() {
                    this->actor = this->scene->GetActor(this->actorId);

                    if (!this->actor) {
                        this->failed = true;

                        return;
                    }

                    this->previousAnimation = this->actor->GetAnimation();
                    this->previousAnimationRestored = false;

                    this->actor->SetAnimation(Actor::StringToAnimation(this->animationName));
                    this->actor->timeSinceLastAnimationFrame = 0.0f;
                    this->actor->animationFrame = 0;
                    this->actor->isPlayingAnimation = true;

                    this->started = true;
                }

                void AnimateActor::Update(float deltaTime) {
                    if (this->started) {
                        this->elapsedTime += deltaTime;
                        this->actor->timeSinceLastAnimationFrame += deltaTime;

                        if (this->actor->timeSinceLastAnimationFrame >= 0.125f) {
                            this->actor->animationFrame = (this->actor->animationFrame + 1) % this->actor->GetAnimationFrameCount();

                            this->actor->timeSinceLastAnimationFrame = 0.0f;
                        }

                        if (this->elapsedTime >= this->duration && !this->previousAnimationRestored) {
                            this->actor->SetAnimation(this->previousAnimation);
                            this->actor->isPlayingAnimation = false;

                            this->previousAnimationRestored = true;
                        }
                    }
                }

                bool AnimateActor::IsCompleted() const {
                    return this->failed || (this->started && this->elapsedTime >= this->duration);
                }
            }
        }
    }
}
