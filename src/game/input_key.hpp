#ifndef SDL03_Game_InputKey
#define SDL03_Game_InputKey

namespace Game {
    // TODO: Rename this to something like InputAction or something since it's not really a key. It's more of an abstract representation of an input that can be mapped to different keys or buttons.   
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
