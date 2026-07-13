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

                for (const auto& choice : this->page->choices) {
                    this->branches[choice.first] = this->scriptRunner->currentScript->nodes[choice.first];
                }

                this->scriptRunner->dialogueSession.Start(*(this->page.get()), true);
            }

            void ChoiceStep::Update(const float deltaTime) {
                this->scriptRunner->dialogueSession.Update(deltaTime);

                std::string selectedChoice = this->scriptRunner->dialogueSession.GetSelectedChoice();

                this->scriptRunner->currentNode->next = this->branches[selectedChoice];
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
