#ifndef SDL03_Game_Scripts_Steps_ActionStep
#define SDL03_Game_Scripts_Steps_ActionStep

#include "script_step.hpp"

namespace Game {
    namespace Scripts {
        namespace Steps {
            class ActionStep : public ScriptStep {
            public:
                ActionStep();
                ~ActionStep();
                void Start();
                void Update(const float deltaTime) ;
                bool IsCompleted() const;
            };
        }
    }
}

#endif
