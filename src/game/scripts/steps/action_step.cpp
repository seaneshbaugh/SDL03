#include "action_step.hpp"

namespace Game {
    namespace Scripts {
        namespace Steps {
            ActionStep::ActionStep() : scriptRunner(nullptr) {
            }

            ActionStep::~ActionStep() {
            }

            void ActionStep::Start(Scripts::ScriptRunner* scriptRunner) {
                this->scriptRunner = scriptRunner;

                if (this->action) {
                    this->action->Start();
                }
            }

            void ActionStep::Update(const float deltaTime) {
                if (this->action) {
                    this->action->Update(deltaTime);
                }
            }

            void ActionStep::Render() {
            }

            bool ActionStep::IsCompleted() const {
                return this->action ? this->action->IsCompleted() : true;
            }
        }
    }
}
