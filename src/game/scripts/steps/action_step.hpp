#ifndef SDL03_Game_Scripts_Steps_ActionStep
#define SDL03_Game_Scripts_Steps_ActionStep

#include <memory>
#include <vector>

#include "../../actions/base.hpp"
#include "script_step.hpp"

namespace Game {
    namespace Scripts {
        namespace Steps {
            class ActionStep : public ScriptStep {
            public:
                std::shared_ptr<Actions::Base> action;
                Scripts::ScriptRunner* scriptRunner;

                ActionStep();
                ~ActionStep();
                void Start(Scripts::ScriptRunner* scriptRunner) override;
                void Update(const float deltaTime) override;
                void Render() override;
                bool IsCompleted() const override;
            };
        }
    }
}

#endif
