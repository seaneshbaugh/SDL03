#ifndef SDL03_Game_Scenes_Controllers_ScriptedController
#define SDL03_Game_Scenes_Controllers_ScriptedController

#include "actor_controller.hpp"
#include "../actor.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            class ScriptedController : public ActorController {
            public:
                ScriptedController(Actor* actor);
                ~ScriptedController();
                void Update(float deltaTime) override;
            private:
                Actor* actor;
            };
        }
    }
}

#endif
