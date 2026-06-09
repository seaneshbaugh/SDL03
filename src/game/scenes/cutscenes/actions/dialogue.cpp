#include "dialogue.hpp"
#include "../../../interfaces/dialogue_manager.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                Dialogue::Dialogue(Interfaces::DialogueManager* dialogueManager, const std::string& dialogueId) : dialogueManager(dialogueManager), dialogueId(dialogueId), started(false) {
                }

                void Dialogue::Start() {
                    this->dialogueManager->StartDialogue(this->dialogueId);

                    this->started = true;
                }

                void Dialogue::Update(float deltaTime) {
                }

                bool Dialogue::IsCompleted() const {
                    return this->dialogueManager->IsDialogueCompleted();
                }
            }
        }
    }
}
