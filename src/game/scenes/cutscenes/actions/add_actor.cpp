#include "add_actor.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                AddActor::AddActor(Scenes::Scene* scene, const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const int x, const int y, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName) : scene(scene), id(id), name(name), spritesheetName(spritesheetName), dialogueId(dialogueId), x(x), y(y), direction(direction), movementScriptName(movementScriptName), interactionScriptName(interactionScriptName) {
                }

                AddActor::~AddActor() {
                }

                void AddActor::Start() {
                    this->scene->AddActor<Scenes::Controllers::CutsceneController>(this->id, this->name, this->spritesheetName, this->dialogueId, this->x, this->y, this->direction, this->movementScriptName, this->interactionScriptName);
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
