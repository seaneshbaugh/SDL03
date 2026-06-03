#ifndef SDL03_Game_Scenes_Controllers_CutsceneController
#define SDL03_Game_Scenes_Controllers_CutsceneController

#include "actor_controller.hpp"
#include "../actor.hpp"

namespace Game {
    namespace Scenes {
        namespace Controllers {
            class CutsceneController : public ActorController {
            public:
                CutsceneController(Actor* actor);
                ~CutsceneController();
                void Update(float deltaTime) override;
            private:
                Actor* actor;
            };
        }
    }
}

#endif
