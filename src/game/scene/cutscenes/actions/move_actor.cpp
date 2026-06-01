#include "move_actor.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                MoveActor::MoveActor(States::Map* map, const std::string& actorId, const std::vector<Actor::Direction>& path) : map(map), actorId(actorId), path(path), started(false) {
                }

                void MoveActor::Start() {
                    this->actor = this->map->actorManager.GetActor(actorId);

                    if (!this->actor) {
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
                    return this->started && !this->actor->IsMoving() && !this->actor->HasPendingMovement();
                }
            }
        }
    }
}
