#include "remove_actor.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                RemoveActor::RemoveActor(States::Map* map, const std::string& actorId) : map(map), actorId(actorId), completed(false) {
                }

                RemoveActor::~RemoveActor() {
                }

                void RemoveActor::Start() {
                    this->map->actorManager.RemoveActor(this->actorId);

                    this->completed = true;
                }

                void RemoveActor::Update(float deltaTime) {
                }

                bool RemoveActor::IsCompleted() const {
                    return this->completed;
                }
            }
        }
    }
}
