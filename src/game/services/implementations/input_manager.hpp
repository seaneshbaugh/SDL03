#ifndef SDL03_Game_Services_Implementations_InputManager
#define SDL03_Game_Services_Implementations_InputManager

#include "../locator.hpp"
#include "../interfaces/input_service.hpp"
#include "../../input_mapper.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class InputManager : public Interfaces::InputService {
            public:
                InputManager();
                ~InputManager();
                InputKey GetInputMapKey(const SDL_Event& event);

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                std::shared_ptr<InputMapper> inputMapper;
            };
        }
    }
}

#endif
