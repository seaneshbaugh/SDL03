#include "scripted_controller.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            ScriptedController::ScriptedController(Actor* actor) : actor(actor) {
            }

            ScriptedController::~ScriptedController() {
            }

            void ScriptedController::Update(float deltaTime) {
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
