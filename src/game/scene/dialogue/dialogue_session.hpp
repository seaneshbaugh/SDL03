#ifndef SDL03_Game_Scene_Dialogue_Session
#define SDL03_Game_Scene_Dialogue_Session

#include <string>

namespace Game {
    namespace Scene {
        namespace Dialogue {
            class DialogueSession {
            public:
                DialogueSession();
                ~DialogueSession();

                int selectedChoice;
                bool completed;
                std::string visibleText;
                float characterTimer;
            private:
            };
        }
    }
}

#endif
