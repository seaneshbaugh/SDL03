#include "cutscene_controller.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            CutsceneController::CutsceneController(Actor* actor) : actor(actor) {
            }

            CutsceneController::~CutsceneController() {
            }

            void CutsceneController::Update(float deltaTime) {
            }
        }
    }
}
