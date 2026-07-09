#ifndef SDL03_Game_Scripts_Steps_DialogueStep
#define SDL03_Game_Scripts_Steps_DialogueStep

#include "script_step.hpp"

namespace Game {
    namespace Scripts {
        namespace Steps {
            class DialogueStep : public ScriptStep {
            public:
                DialogueStep();
                ~DialogueStep();
                void Start();
                void Update(const float deltaTime) ;
                bool IsCompleted() const;
            };
        }
    }
}

#endif
