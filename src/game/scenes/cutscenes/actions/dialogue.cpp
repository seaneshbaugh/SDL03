#include "dialogue.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                Dialogue::Dialogue(States::Map* map, const std::string& dialogueId) : map(map), dialogueId(dialogueId), started(false) {
                }

                void Dialogue::Start() {
                    this->map->StartDialogue(this->dialogueId);

                    this->started = true;
                }

                void Dialogue::Update(float deltaTime) {
                }

                bool Dialogue::IsCompleted() const {
                    return this->map->DialogueSessionCompleted();
                }
            }
        }
    }
}
