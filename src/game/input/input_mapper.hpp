#ifndef SDL03_Game_Input_InputMapper
#define SDL03_Game_Input_InputMapper

#include <map>

#include <SDL3/SDL.h>

#include "button.hpp"
#include "../services/locator.hpp"

namespace Game {
    namespace Input {
        class InputMapper {
        public:
            static const std::map<int, Button> defaultKeyboardMap;
            static const std::map<int, Button> defaultGamepadMap;

            InputMapper();
            InputMapper(const std::map<int, Button>& keyboardMap);
            ~InputMapper();
            std::map<int, Button>::size_type MapButtons(const std::map<int, Button>& keys);
            Button SetInputMapButton(const int& rawKeyValue, const Button& inputValue);
            Button GetButton(const SDL_Event& event);
            Button GetButton(const int& rawKeyValue);
        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            std::map<int, Button> keyboardMap;
            std::map<int, Button> gamepadMap;

            void SetDefaultInputMap();
            static std::string GamepadButtonToString(int gamepadButton);
        };
    }
}

#endif
