#ifndef SDL03_Game_InputMapper
#define SDL03_Game_InputMapper

#include <map>

#include "SDL.h"

#include "input_key.hpp"
#include "services/locator.hpp"

namespace Game {
    class InputMapper {
    public:
        InputMapper();
        InputMapper(const std::map<int, InputKey>& keys);
        ~InputMapper();
        std::map<int, InputKey>::size_type MapKeys(const std::map<int, InputKey>& keys);
        InputKey SetInputMapKey(const int& rawKeyValue, const InputKey& inputValue);
        InputKey GetInputMapKey(const SDL_Event& event);
    private:
        static const std::string logChannel;

        std::shared_ptr<Log::Logger> logger;
        std::map<int, InputKey> inputMap;

        void SetDefaultInputMap();
    };
}

#endif
