#ifndef SDL03_Game_Input_InputState
#define SDL03_Game_Input_InputState

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
        };
    }
}

#endif
