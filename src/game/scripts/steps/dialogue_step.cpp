#include "dialogue_step.hpp"
#include "../script_runner.hpp"

namespace Game {
    namespace Scripts {
        namespace Steps {
            DialogueStep::DialogueStep() : actorId(std::nullopt), page(nullptr), nextIndicatorVisible(false), scriptRunner(nullptr) {
            }

            DialogueStep::~DialogueStep() {
            }

            void DialogueStep::Start(Scripts::ScriptRunner* scriptRunner) {
                this->scriptRunner = scriptRunner;

                this->scriptRunner->dialogueSession.Start(*(this->page.get()), true);
            }

            void DialogueStep::Update(const float deltaTime) {
                this->scriptRunner->dialogueSession.Update(deltaTime);
            }

            void DialogueStep::Render() {
                this->scriptRunner->dialogueSession.Render(this->scriptRunner->currentScript->scene->camera);
            }

            bool DialogueStep::IsCompleted() const {
                return this->scriptRunner->dialogueSession.IsCompleted();
            }
        }
    }
}
