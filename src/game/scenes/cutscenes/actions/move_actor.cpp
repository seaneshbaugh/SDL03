#include "move_actor.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                MoveActor::MoveActor(States::Map* map, const std::string& actorId, const std::vector<Actor::Direction>& path) : map(map), actorId(actorId), path(path), started(false), failed(false) {
                }

                void MoveActor::Start() {
                    this->actor = this->map->scene->actorManager->GetActor(this->actorId);

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
