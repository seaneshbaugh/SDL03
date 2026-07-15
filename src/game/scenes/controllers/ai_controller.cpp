#include "ai_controller.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            AIController::AIController(Actor* actor) : actor(actor) {
            }

            AIController::~AIController() {
            }

            void AIController::Update(float deltaTime) {
                sol::protected_function update = (*this->actor->luaState.get())["update"];

                if (update.valid()) {
                    sol::protected_function_result result = update(deltaTime);

                    if (!result.valid()) {
                        sol::error e = result;
                        // TODO: Add logging to actor controllers.
                        //this->logger->error() << "Error in Lua update function: " << e.what();
                    }
                }
            }
        }
    }
}
