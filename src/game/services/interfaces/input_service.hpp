#ifndef SDL03_Game_Services_Interfaces_InputService
#define SDL03_Game_Services_Interfaces_InputService

#include <SDL3/SDL.h>

#include "../../input/button.hpp"
#include "../../input/input_state.hpp"

namespace Game {
    namespace Services {
        namespace Interfaces {
            class InputService {
            public:
                virtual ~InputService() {}
                virtual Input::Button GetButton(const SDL_Event& event) = 0;
                virtual void HandleEvent(const SDL_Event& event) = 0;
                virtual void BeginFrame() = 0;
                virtual void EndFrame() = 0;
                virtual const Input::InputState& GetCurrentInputState() const = 0;
            };
        }
    }
}

#endif
