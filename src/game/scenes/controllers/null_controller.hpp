#ifndef SDL03_Game_Scenes_Controllers_NullController
#define SDL03_Game_Scenes_Controllers_NullController

#include "actor_controller.hpp"
#include "../actor.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            class NullController : public ActorController {
            public:
                NullController(Actor* actor);
                ~NullController();
                void Update(float deltaTime) override;
            private:
                Actor* actor;
            };
        }
    }
}

#endif
