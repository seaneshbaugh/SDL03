#include "dialogue_manager.hpp"

namespace Game {
    namespace Interfaces {
        bool DialogueManager::IsDialogueCompleted() const {
            return this->dialogueSession.IsCompleted();
        }
    }
}
