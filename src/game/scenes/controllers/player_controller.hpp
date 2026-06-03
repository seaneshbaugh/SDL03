#ifndef SDL03_Game_Scenes_Controllers_PlayerController
#define SDL03_Game_Scenes_Controllers_PlayerController

#include "actor_controller.hpp"
#include "../actor.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            class PlayerController : public ActorController {
            public:
                PlayerController(Actor* actor);
                ~PlayerController();
                void Update(float deltaTime) override;
            private:
                Actor* actor;
            };
        }
    }
}

#endif
