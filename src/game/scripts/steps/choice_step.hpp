#ifndef SDL03_Game_Scripts_Steps_ChoiceStep
#define SDL03_Game_Scripts_Steps_ChoiceStep

#include "../dialogue/dialogue_page.hpp"
#include "../dialogue/dialogue_session.hpp"
#include "script_step.hpp"

namespace Game {
    namespace Scripts {
        class ScriptNode;
        class ScriptRunner;

        namespace Steps {
            class ChoiceStep : public ScriptStep {
            public:
                std::optional<std::string> actorId;
                std::unique_ptr<Dialogue::DialoguePage> page;
                Scripts::ScriptRunner* scriptRunner;

                ChoiceStep();
                ~ChoiceStep();
                void Start(Scripts::ScriptRunner* scriptRunner) override;
                void Update(const float deltaTime) override;
                void Render() override;
                bool IsCompleted() const override;

            private:
                std::map<std::string, std::shared_ptr<Scripts::ScriptNode>> branches;
            };
        }
    }
}

#endif
