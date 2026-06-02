#ifndef SDL03_Game_Scene_Cutscenes_CutsceneSession
#define SDL03_Game_Scene_Cutscenes_CutsceneSession

#include <string>

#include "cutscene.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            class CutsceneSession {
            public:
                CutsceneSession();
                ~CutsceneSession();
                void Start(std::shared_ptr<Cutscene> cutscene);
                void Update(const float deltaTime);
                bool IsCompleted() const;

            private:
                std::shared_ptr<Cutscene> currentCutscene;
                std::queue<std::shared_ptr<Actions::Base>> actions;
                std::shared_ptr<Actions::Base> currentAction;
                bool completed;
            };
        }
    }
}

#endif
