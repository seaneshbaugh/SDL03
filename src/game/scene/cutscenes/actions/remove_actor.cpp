#include "remove_actor.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                RemoveActor::RemoveActor(States::Map* map, const std::string& actorId) : map(map), actorId(actorId) {
                }

                RemoveActor::~RemoveActor() {
                }

                void RemoveActor::Start() {
                    this->map->RemoveActor(this->actorId);
                }

                void RemoveActor::Update(float deltaTime) {
                }

                bool RemoveActor::IsCompleted() const {
                    return true;
                }
            }
        }
    }
}
