#ifndef SDL03_Game_Services_Interfaces_InputService
#define SDL03_Game_Services_Interfaces_InputService

#include "SDL.h"

#include "../../input_key.hpp"

namespace Game {
    namespace Services {
        namespace Interfaces {
            class InputService {
            public:
                virtual ~InputService() {}
                virtual InputKey GetInputMapKey(const SDL_Event& event) = 0;
            };
        }
    }
}

#endif
