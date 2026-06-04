#ifndef SDL03_Game_Input_InputState
#define SDL03_Game_Input_InputState

#include <memory>

#include "../../../lib/sol/sol.hpp"

namespace Game {
    namespace Input {
        struct InputState {
            bool upPressed = false;
            bool upHeld = false;
            
            bool downPressed = false;
            bool downHeld = false;
            
            bool leftPressed = false;
            bool leftHeld = false;
            
            bool rightPressed = false;
            bool rightHeld = false;

            bool confirmPressed = false;
            bool confirmHeld = false;

            bool cancelPressed = false;
            bool cancelHeld = false;

            bool skipPressed = false;
            bool skipHeld = false;

            bool switchPressed = false;
            bool switchHeld = false;

            bool menuPressed = false;
            bool menuHeld = false;

            bool scrollPressed = false;
            bool scrollHeld = false;

            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<sol::state> luaState);
            };
        };
    }
}

#endif
