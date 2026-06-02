#include "cutscene_session.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            CutsceneSession::CutsceneSession() : currentCutscene(nullptr), completed(false) {
            }

            CutsceneSession::~CutsceneSession() {
            }

            void CutsceneSession::Start(std::shared_ptr<Cutscene> cutscene) {
                this->currentCutscene = cutscene;
                this->actions = std::queue<std::shared_ptr<Actions::Base>>();
                this->currentAction = nullptr;
                this->completed = false;

                for (auto& action : cutscene->actions) {
                    this->actions.push(action);
                }
            }

            void CutsceneSession::Update(const float deltaTime) {
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

            bool CutsceneSession::IsCompleted() const {
                return this->completed;
            }
        }
    }
}
