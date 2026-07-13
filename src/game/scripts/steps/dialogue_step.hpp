#ifndef SDL03_Game_Scripts_Steps_DialogueStep
#define SDL03_Game_Scripts_Steps_DialogueStep

#include "../dialogue/dialogue_page.hpp"
#include "../dialogue/dialogue_session.hpp"
#include "script_step.hpp"

namespace Game {
    namespace Scripts {
        namespace Steps {
            class DialogueStep : public ScriptStep {
            public:
                std::optional<std::string> actorId;
                std::unique_ptr<Dialogue::DialoguePage> page;
                bool nextIndicatorVisible;
                Scripts::ScriptRunner* scriptRunner;

                DialogueStep();
                ~DialogueStep();
                void Start(Scripts::ScriptRunner* scriptRunner) override;
                void Update(const float deltaTime) override;
                void Render() override;
                bool IsCompleted() const override;
            };
        }
    }
}

#endif
