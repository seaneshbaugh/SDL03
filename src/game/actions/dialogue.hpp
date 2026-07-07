#ifndef SDL03_Game_Actions_Dialogue
#define SDL03_Game_Actions_Dialogue

#include <memory>

#include "base.hpp"

namespace Game {
    namespace Interfaces {
        class DialogueManager;
    }

    namespace Actions {
        class Dialogue : public Base {
        public:
            Dialogue(Interfaces::DialogueManager* dialogueManager, const std::string& dialogueId);
            virtual ~Dialogue() = default;
            void Start() override;
            void Update(float deltaTime) override;
            bool IsCompleted() const override;

        private:
            Interfaces::DialogueManager* dialogueManager;
            std::string dialogueId;
            bool started;
        };
    }
}

#endif
