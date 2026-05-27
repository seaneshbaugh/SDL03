#include "cutscene_session.hpp"

namespace Game {
    namespace Scene {
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

            // This only handles blocking actions. At some point very soon I'm going to need
            // to figure out how to make actions asynchronous so I can have something like
            // two NPCs moving around at the same time while dialogue is happening.
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
