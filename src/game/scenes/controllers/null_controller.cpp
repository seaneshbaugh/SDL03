#include "null_controller.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            NullController::NullController(Actor* actor) : actor(actor) {
            }

            NullController::~NullController() {
            }

            void NullController::Update(float deltaTime) {
            }
        }
    }
}
