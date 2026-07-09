#ifndef SDL03_Game_Scripts_Steps_ChoiceStep
#define SDL03_Game_Scripts_Steps_ChoiceStep

#include "script_step.hpp"

namespace Game {
    namespace Scripts {
        namespace Steps {
            class ChoiceStep : public ScriptStep {
            public:
                ChoiceStep();
                ~ChoiceStep();
                void Start();
                void Update(const float deltaTime);
                bool IsCompleted() const;
            };
        }
    }
}

#endif
