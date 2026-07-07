#include "action_runner.hpp"

namespace Game {
    namespace Actions {
        ActionRunner::ActionRunner() : currentAction(nullptr), completed(false) {
        }

        ActionRunner::~ActionRunner() {
        }

        void ActionRunner::Start(std::vector<std::shared_ptr<Actions::Base>> actions) {
            this->actions = std::queue<std::shared_ptr<Actions::Base>>();
            this->currentAction = nullptr;
            this->completed = false;

            for (auto& action : actions) {
                this->actions.push(action);
            }
        }

        void ActionRunner::Update(const float deltaTime) {
            if (!this->currentAction) {
                if (this->actions.empty()) {
                    this->completed = true;

                    return;
                } else {
                    this->currentAction = this->actions.front();
                    this->actions.pop();

                    this->currentAction->Start();
                }
            }

            currentAction->Update(deltaTime);

            if (this->currentAction->IsCompleted()) {
                this->currentAction = nullptr;
            }
        }

        bool ActionRunner::IsCompleted() const {
            return this->completed;
        }
    }
}
