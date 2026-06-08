#include "remove_actor.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                RemoveActor::RemoveActor(Scenes::Scene* scene, const std::string& actorId) : scene(scene), actorId(actorId), completed(false) {
                }

                RemoveActor::~RemoveActor() {
                }

                void RemoveActor::Start() {
                    this->scene->RemoveActor(this->actorId);

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
