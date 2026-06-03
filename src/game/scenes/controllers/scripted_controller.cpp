#include "scripted_controller.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            ScriptedController::ScriptedController(Actor* actor) : actor(actor) {
            }

            ScriptedController::~ScriptedController() {
            }

            void ScriptedController::Update(float deltaTime) {
            }
        }
    }
}
