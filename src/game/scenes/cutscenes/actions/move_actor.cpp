#include "move_actor.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                MoveActor::MoveActor(Scenes::Scene* scene, const std::string& actorId, const std::vector<Actor::Direction>& path) : scene(scene), actorId(actorId), path(path), started(false), failed(false) {
                }

                void MoveActor::Start() {
                    this->actor = this->scene->GetActor(this->actorId);

                    if (!this->actor) {
                        this->failed = true;

                        return;
                    }

                    actor->ClearPendingMovement();

                    for (auto direction : this->path) {
                        actor->QueueMovement(direction);
                    }

                    this->started = true;
                }

                void MoveActor::Update(float deltaTime) {
                }

                bool MoveActor::IsCompleted() const {
                    return this->failed || (this->started && !this->actor->IsMoving() && !this->actor->HasPendingMovement());
                }
            }
        }
    }
}
