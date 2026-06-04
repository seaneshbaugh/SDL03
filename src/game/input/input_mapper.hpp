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
            InputMapper();
            InputMapper(const std::map<int, Button>& keys);
            ~InputMapper();
            std::map<int, Button>::size_type MapButtons(const std::map<int, Button>& keys);
            Button SetInputMapButton(const int& rawKeyValue, const Button& inputValue);
            Button GetInputButton(const SDL_Event& event);
            Button GetInputButton(const int& rawKeyValue);
        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            std::map<int, Button> inputMap;

            void SetDefaultInputMap();
        };
    }
}

#endif
