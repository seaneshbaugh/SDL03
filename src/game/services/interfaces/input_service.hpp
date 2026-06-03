#ifndef SDL03_Game_Services_Interfaces_InputService
#define SDL03_Game_Services_Interfaces_InputService

#include <SDL3/SDL.h>

#include "../../input/button.hpp"

namespace Game {
    namespace Services {
        namespace Interfaces {
            class InputService {
            public:
                virtual ~InputService() {}
                virtual Input::Button GetInputButton(const SDL_Event& event) = 0;
            };
        }
    }
}

#endif
