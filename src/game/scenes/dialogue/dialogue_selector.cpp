#include "dialogue_selector.hpp"

namespace Game {
    namespace Scenes {
        namespace Dialogue {
            DialogueSelector::DialogueSelector(const std::string& dialogueProfileId) : dialogueProfile(dialogueProfileId) {
            }

            DialogueSelector::~DialogueSelector() {
            }

            std::string DialogueSelector::SelectDialogue(const World::Conditions::Contexts::Context& context) {
                for (const auto& entry : this->dialogueProfile.entries) {
                    if (!entry.condition) {
                        return entry.dialogueId;
                    }

                    if (entry.condition->Evaluate(context)) {
                        return entry.dialogueId;
                    }
                }

                // Make this return an optional?
                return "";
            }
        }
    }
}
