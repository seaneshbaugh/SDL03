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
                this->inputMapper = std::make_shared<InputMapper>(Locator::SettingsService()->InputSettings());
            }

            InputManager::~InputManager() {
            }

            InputKey InputManager::GetInputMapKey(const SDL_Event& event) {
                try {
                    if (event.type == SDL_KEYDOWN) {
                        return this->inputMapper->GetInputMapKey(event);
                    } else {
                        return InputKey::NO_KEY;
                    }
                } catch (const std::out_of_range& exception) {
                    return InputKey::NO_KEY;
                }
            }
        }
    }
}
