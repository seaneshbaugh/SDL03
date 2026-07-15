#ifndef SDL03_Game_Scenes_Controllers_AIController
#define SDL03_Game_Scenes_Controllers_AIController

#include "actor_controller.hpp"
#include "../actor.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            class AIController : public ActorController {
            public:
                AIController(Actor* actor);
                ~AIController();
                void Update(float deltaTime) override;
            private:
                Actor* actor;
            };
        }
    }
}

#endif
