#ifndef SDL03_Game_Interfaces_DialogueManager
#define SDL03_Game_Interfaces_DialogueManager

#include "../scenes/dialogue/dialogue_session.hpp"

namespace Game {
    namespace Interfaces {
        class DialogueManager {
        public:
            virtual ~DialogueManager() = default;
            virtual void StartDialogue(const std::string& dialogueId) = 0;
            virtual bool IsDialogueCompleted() const;

        protected:
            Scenes::Dialogue::DialogueSession dialogueSession;
        };
    }
}

#endif
