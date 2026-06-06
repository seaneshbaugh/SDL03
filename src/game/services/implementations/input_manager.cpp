#include "input_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string InputManager::logChannel = "input";
            const int InputManager::gamepadAxisDeadZone = 8000;

            InputManager::InputManager() : gamepad(nullptr) {
                if (Locator::LoggerService() == nullptr) {
                    throw std::runtime_error("LoggerService must be started before InputService.");
                }

                this->logger = Locator::LoggerService()->GetLogger(InputManager::logChannel);
                this->inputMapper = std::make_shared<Input::InputMapper>(Locator::SettingsService()->InputSettings());
            }

            InputManager::~InputManager() {
            }

            Input::Button InputManager::GetButton(const SDL_Event& event) {
                if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                    return this->inputMapper->GetButton(event);
                } else {
                    return Input::Button::None;
                }
            }

            void InputManager::HandleEvent(const SDL_Event& event) {
                Input::Button button = this->inputMapper->GetButton(event);

                switch (event.type) {
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.repeat) {
                        break;
                    }

                    [[fallthrough]];
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                    switch (button) {
                    case Input::Button::Up:
                        this->inputState.upPressed = true;
                        this->inputState.upHeld = true;

                        break;
                    case Input::Button::Down:
                        this->inputState.downPressed = true;
                        this->inputState.downHeld = true;

                        break;
                    case Input::Button::Left:
                        this->inputState.leftPressed = true;
                        this->inputState.leftHeld = true;

                        break;
                    case Input::Button::Right:
                        this->inputState.rightPressed = true;
                        this->inputState.rightHeld = true;

                        break;
                    case Input::Button::Confirm:
                        this->inputState.confirmPressed = true;
                        this->inputState.confirmHeld = true;

                        break;
                    case Input::Button::Cancel:
                        this->inputState.cancelPressed = true;
                        this->inputState.cancelHeld = true;

                        break;
                    case Input::Button::Skip:
                        this->inputState.skipPressed = true;
                        this->inputState.skipHeld = true;

                        break;
                    case Input::Button::Switch:
                        this->inputState.switchPressed = true;
                        this->inputState.switchHeld = true;

                        break;
                    case Input::Button::Menu:
                        this->inputState.menuPressed = true;
                        this->inputState.menuHeld = true;

                        break;
                    case Input::Button::Scroll:
                        this->inputState.scrollPressed = true;
                        this->inputState.scrollHeld = true;

                        break;
                    }

                    break;
                case SDL_EVENT_KEY_UP:
                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                    switch (button) {
                    case Input::Button::Up:
                        this->inputState.upHeld = false;

                        break;
                    case Input::Button::Down:
                        this->inputState.downHeld = false;

                        break;
                    case Input::Button::Left:
                        this->inputState.leftHeld = false;

                        break;
                    case Input::Button::Right:
                        this->inputState.rightHeld = false;

                        break;
                    case Input::Button::Confirm:
                        this->inputState.confirmHeld = false;

                        break;
                    case Input::Button::Cancel:
                        this->inputState.cancelHeld = false;

                        break;
                    case Input::Button::Skip:
                        this->inputState.skipHeld = false;

                        break;
                    case Input::Button::Switch:
                        this->inputState.switchHeld = false;

                        break;
                    case Input::Button::Menu:
                        this->inputState.menuHeld = false;

                        break;
                    case Input::Button::Scroll:
                        this->inputState.scrollHeld = false;

                        break;
                    }

                    break;
                case SDL_EVENT_GAMEPAD_AXIS_MOTION:

                    if (event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX) {
                        if (event.gaxis.value < -InputManager::gamepadAxisDeadZone) {
                            this->inputState.leftPressed = true;
                            this->inputState.leftHeld = true;
                        } else if (event.gaxis.value > InputManager::gamepadAxisDeadZone) {
                            this->inputState.rightPressed = true;
                            this->inputState.rightHeld = true;
                        } else {
                            this->inputState.leftHeld = false;
                            this->inputState.rightHeld = false;
                        }
                    } else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY) {
                        if (event.gaxis.value < -InputManager::gamepadAxisDeadZone) {
                            this->inputState.upPressed = true;
                            this->inputState.upHeld = true;
                        } else if (event.gaxis.value > InputManager::gamepadAxisDeadZone) {
                            this->inputState.downPressed = true;
                            this->inputState.downHeld = true;
                        } else {
                            this->inputState.upHeld = false;
                            this->inputState.downHeld = false;
                        }
                    }

                    break;
                case SDL_EVENT_GAMEPAD_ADDED:
                    // TODO: Handle multiple gamepads gracefully.
                    if (!this->gamepad) {
                        this->gamepad = SDL_OpenGamepad(event.gdevice.which);

                        this->logger->debug() << "Added gamepad " << SDL_GetGamepadName(this->gamepad) << " with ID " << event.gdevice.which;
                    }

                    break;
                case SDL_EVENT_GAMEPAD_REMOVED:
                    if (this->gamepad && SDL_GetGamepadFromID(event.gdevice.which) == this->gamepad) {
                        SDL_CloseGamepad(this->gamepad);

                        this->logger->debug() << "Removed gamepad " << SDL_GetGamepadName(this->gamepad) << " with ID " << event.gdevice.which;

                        this->gamepad = nullptr;
                    }

                    break;
                }
            }

            void InputManager::BeginFrame() {
                this->inputState.upPressed = false;
                this->inputState.downPressed = false;
                this->inputState.leftPressed = false;
                this->inputState.rightPressed = false;
                this->inputState.confirmPressed = false;
                this->inputState.cancelPressed = false;
                this->inputState.skipPressed = false;
                this->inputState.switchPressed = false;
                this->inputState.menuPressed = false;
                this->inputState.scrollPressed = false;
            }

            void InputManager::EndFrame() {

            }

            const Input::InputState& InputManager::GetCurrentInputState() const {
                return this->inputState;
            }
        }
    }
}
