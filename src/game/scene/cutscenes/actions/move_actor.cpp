#include "move_actor.hpp"

namespace Game {
    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                MoveActor::MoveActor(std::shared_ptr<Actor> actor, const std::vector<Actor::Direction>& path) : actor(actor), path(path), started(false) {
                }

                void MoveActor::Start() {
                    actor->ClearPendingMovement();

                    for (auto direction : this->path) {
                        actor->QueueMovement(direction);
                    }

                    this->started = true;
                }

                void MoveActor::Update(float deltaTime) {
                }

                bool MoveActor::IsCompleted() const {
                    return this->started && !this->actor->IsMoving() && !this->actor->HasPendingMovement();
                }
            }
        }
    }
}
