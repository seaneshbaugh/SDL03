#include "animate_actor.hpp"
#include "../states/map.hpp"

namespace Game {
    namespace Actions {
AnimateActor::AnimateActor(Scenes::Scene* scene, const std::string& actorId, const std::string& animationName, int loops) : scene(scene), actorId(actorId), animationName(animationName), loops(loops), currentLoop(0), started(false), failed(false), previousAnimationClip(nullptr) {
        }

        AnimateActor::~AnimateActor() {
        }

        void AnimateActor::Start() {
            this->actor = this->scene->GetActor(this->actorId);

            if (!this->actor) {
                this->failed = true;

                return;
            }

            this->previousAnimationClip = this->actor->animationPlayer.GetCurrentAnimationClip();

            this->actor->animationPlayer.Play(this->actor->appearance->spritesheet->GetAnimationClip(this->animationName));

            this->started = true;
        }

        void AnimateActor::Update(float deltaTime) {
            if (!this->started || this->failed) {
                return;
            }

            if (this->actor->animationPlayer.ConsumeCompletedLoop()) {
                this->currentLoop++;

                if (this->currentLoop >= this->loops) {
                    this->actor->animationPlayer.Play(this->previousAnimationClip);
                }
            }
        }

        bool AnimateActor::IsCompleted() const {
            return this->failed || (this->started && this->currentLoop >= this->loops);
        }
    }
}
