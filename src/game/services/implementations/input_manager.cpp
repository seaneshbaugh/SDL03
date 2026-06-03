#include "input_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string InputManager::logChannel = "input";

            InputManager::InputManager() {
                if (Locator::LoggerService() == nullptr) {
                    throw std::runtime_error("LoggerService must be started before InputService.");
                }

                this->logger = Locator::LoggerService()->GetLogger(InputManager::logChannel);
                this->inputMapper = std::make_shared<Input::InputMapper>(Locator::SettingsService()->InputSettings());
            }

            InputManager::~InputManager() {
            }

            Input::Button InputManager::GetInputButton(const SDL_Event& event) {
                //if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
                //    this->logger->debug() << "Button: " << static_cast<int>(event.gbutton.button);
                //}

                try {
                    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                        return this->inputMapper->GetInputButton(event);
                    } else {
                        return Input::Button::None;
                    }
                } catch (const std::out_of_range& exception) {
                    return Input::Button::None;
                }
            }
        }
    }
}
