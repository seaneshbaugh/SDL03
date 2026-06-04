#include "input_state.hpp"

namespace Game {
    namespace Input {
        void InputState::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            (*luaState.get()).new_usertype<InputState>("InputState",
                                                       sol::no_constructor,
                                                       "up_pressed", &InputState::upPressed,
                                                       "up_held", &InputState::upHeld,
                                                       "down_pressed", &InputState::downPressed,
                                                       "down_held", &InputState::downHeld,
                                                       "left_pressed", &InputState::leftPressed,
                                                       "left_held", &InputState::leftHeld,
                                                       "right_pressed", &InputState::rightPressed,
                                                       "right_held", &InputState::rightHeld,
                                                       "confirm_pressed", &InputState::confirmPressed,
                                                       "confirm_held", &InputState::confirmHeld,
                                                       "cancel_pressed", &InputState::cancelPressed,
                                                       "cancel_held", &InputState::cancelHeld,
                                                       "skip_pressed", &InputState::skipPressed,
                                                       "skip_held", &InputState::skipHeld,
                                                       "switch_pressed", &InputState::switchPressed,
                                                       "switch_held", &InputState::switchHeld,
                                                       "menu_pressed", &InputState::menuPressed,
                                                       "menu_held", &InputState::menuHeld,
                                                       "scroll_pressed", &InputState::scrollPressed,
                                                       "scroll_held", &InputState::scrollHeld);
        }
    }
}
