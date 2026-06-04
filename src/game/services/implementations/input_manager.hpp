#ifndef SDL03_Game_Services_Implementations_InputManager
#define SDL03_Game_Services_Implementations_InputManager

#include "../locator.hpp"
#include "../interfaces/input_service.hpp"
#include "../../input/input_mapper.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            class InputManager : public Interfaces::InputService {
            public:
                InputManager();
                ~InputManager();
                Input::Button GetInputButton(const SDL_Event& event);
                void HandleEvent(const SDL_Event& event);
                void BeginFrame();
                void EndFrame();
                const Input::InputState& GetCurrentInputState() const;

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                std::shared_ptr<Input::InputMapper> inputMapper;
                Input::InputState inputState;
            };
        }
    }
}

#endif
