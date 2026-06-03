#ifndef SDL03_Game_Input_InputKey
#define SDL03_Game_Input_InputKey

namespace Game {
    namespace Input {
        enum class Button : int {
            None    = 0,
            Up      = 1,
            Down    = 2,
            Left    = 3,
            Right   = 4,
            Confirm = 5,
            Cancel  = 6,
            Skip    = 7,
            Switch  = 8,
            Menu    = 9,
            Scroll  = 10
        };
    }
}

#endif
