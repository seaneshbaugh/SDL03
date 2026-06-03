#ifndef SDL03_Game_Scenes_Controllers_ActorController
#define SDL03_Game_Scenes_Controllers_ActorController

namespace Game {
    namespace Scenes {
        namespace Controllers {
            class ActorController {
            public:
                ~ActorController() = default;
                virtual void Update(float deltaTime) = 0;
            };
        }
    }
}

#endif
