#include "add_actor.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                AddActor::AddActor(States::Map* map, const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const int x, const int y, const Scene::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName) : map(map), id(id), name(name), spritesheetName(spritesheetName), dialogueId(dialogueId), x(x), y(y), direction(direction), movementScriptName(movementScriptName), interactionScriptName(interactionScriptName) {
                }

                AddActor::~AddActor() {
                }

                void AddActor::Start() {
                    this->map->actorManager->AddActor(this->id, this->name, this->spritesheetName, this->dialogueId, this->x, this->y, this->direction, this->movementScriptName, this->interactionScriptName);
                }

                void AddActor::Update(float deltaTime) {
                }

                bool AddActor::IsCompleted() const {
                    return true;
                }
            }
        }
    }
}
