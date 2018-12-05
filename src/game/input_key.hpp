#ifndef SDL03_Game_InputKey
#define SDL03_Game_InputKey

namespace Game {
    enum class InputKey : int {
        NO_KEY      = 0,
        UP_KEY      = 1,
        DOWN_KEY    = 2,
        LEFT_KEY    = 3,
        RIGHT_KEY   = 4,
        CONFIRM_KEY = 5,
        CANCEL_KEY  = 6,
        MENU_KEY    = 7
    };
}

#endif
