#include "cutscene_session.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            CutsceneSession::CutsceneSession() : currentCutscene(nullptr) {
            }

            CutsceneSession::~CutsceneSession() {
            }

            void CutsceneSession::Start(std::shared_ptr<Cutscene> cutscene) {
                this->currentCutscene = cutscene;

                this->actionRunner.Start(cutscene->actions);
            }

            void CutsceneSession::Update(const float deltaTime) {
                this->actionRunner.Update(deltaTime);
            }

            bool CutsceneSession::IsCompleted() const {
                return this->actionRunner.IsCompleted();
            }
        }
    }
}
