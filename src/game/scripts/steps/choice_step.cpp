#include "choice_step.hpp"
#include "../script_runner.hpp"

namespace Game {
    namespace Scripts {
        namespace Steps {
            ChoiceStep::ChoiceStep() : scriptRunner(nullptr) {
            }

            ChoiceStep::~ChoiceStep() {
            }

            void ChoiceStep::Start(Scripts::ScriptRunner* scriptRunner) {
                this->scriptRunner = scriptRunner;

                this->scriptRunner->dialogueSession.Start(*(this->page.get()), true);
            }

            void ChoiceStep::Update(const float deltaTime) {
                this->scriptRunner->dialogueSession.Update(deltaTime);

                if (this->scriptRunner->dialogueSession.IsCompleted()) {
                    this->scriptRunner->JumpToNode(this->scriptRunner->currentScript->nodes[this->scriptRunner->dialogueSession.GetSelectedChoice()]);
                }
            }

            void ChoiceStep::Render() {
                this->scriptRunner->dialogueSession.Render(this->scriptRunner->currentScript->scene->camera);
            }

            bool ChoiceStep::IsCompleted() const {
                return this->scriptRunner->dialogueSession.IsCompleted();
            }
        }
    }
}
