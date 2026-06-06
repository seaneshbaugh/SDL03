#include "input_mapper.hpp"

namespace Game {
    namespace Input {
        const std::map<int, Button> InputMapper::defaultKeyboardMap {
            {SDLK_UP, Button::Up},
            {SDLK_DOWN, Button::Down},
            {SDLK_LEFT, Button::Left},
            {SDLK_RIGHT, Button::Right},
            {SDLK_Z, Button::Confirm},
            {SDLK_X, Button::Cancel},
            {SDLK_A, Button::Skip},
            {SDLK_S, Button::Switch},
            {SDLK_TAB, Button::Menu},
            {SDLK_DELETE, Button::Scroll}};

        const std::map<int, Button> InputMapper::defaultGamepadMap{
            {SDL_GAMEPAD_BUTTON_DPAD_UP, Button::Up},
            {SDL_GAMEPAD_BUTTON_DPAD_DOWN, Button::Down},
            {SDL_GAMEPAD_BUTTON_DPAD_LEFT, Button::Left},
            {SDL_GAMEPAD_BUTTON_DPAD_RIGHT, Button::Right},
            {SDL_GAMEPAD_BUTTON_EAST, Button::Confirm},
            {SDL_GAMEPAD_BUTTON_SOUTH, Button::Cancel},
            {SDL_GAMEPAD_BUTTON_NORTH, Button::Skip},
            {SDL_GAMEPAD_BUTTON_WEST, Button::Switch},
            {SDL_GAMEPAD_BUTTON_START, Button::Menu},
            {SDL_GAMEPAD_BUTTON_GUIDE, Button::Scroll}};

        const std::string InputMapper::logChannel = "input";

        InputMapper::InputMapper() : keyboardMap(defaultKeyboardMap), gamepadMap(defaultGamepadMap) {
            this->logger = Services::Locator::LoggerService()->GetLogger(InputMapper::logChannel);
        }

        InputMapper::InputMapper(const std::map<int, Button>& keyboardMap) : keyboardMap(defaultKeyboardMap), gamepadMap(defaultGamepadMap) {
            // this->MapButtons(keyboardMap);
            this->logger = Services::Locator::LoggerService()->GetLogger(InputMapper::logChannel);
        }

        InputMapper::~InputMapper() {
        }

        std::map<int, Button>::size_type InputMapper::MapButtons(const std::map<int, Button>& buttons) {
            for (auto it = buttons.begin(); it != buttons.end(); ++it) {
                this->SetInputMapButton(it->first, it->second);
            }

            return buttons.size();
        }

        Button InputMapper::SetInputMapButton(const int& rawKeyValue, const Button& inputValue) {
            for (auto it = this->keyboardMap.begin(); it != this->keyboardMap.end();) {
                if (it->second == inputValue) {
                    this->keyboardMap.erase(it++);
                } else {
                    ++it;
                }
            }

            this->keyboardMap[rawKeyValue] = inputValue;

            return inputValue;
        }

        Button InputMapper::GetButton(const SDL_Event& event) {
            try {
                switch (event.type) {
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                    return this->keyboardMap.at(event.key.key);
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                    return this->gamepadMap.at(event.gbutton.button);
                default:
                    return Button::None;
                }
            } catch (const std::out_of_range& exception) {
                return Button::None;
            }
        }

        // TODO: Figure out a better place for this.
        void InputMapper::SetDefaultInputMap() {
            this->keyboardMap[SDLK_UP] = Button::Up;
            this->keyboardMap[SDLK_DOWN] = Button::Down;
            this->keyboardMap[SDLK_LEFT] = Button::Left;
            this->keyboardMap[SDLK_RIGHT] = Button::Right;
            this->keyboardMap[SDLK_Z] = Button::Confirm;
            this->keyboardMap[SDLK_X] = Button::Cancel;
            this->keyboardMap[SDLK_A] = Button::Skip;
            this->keyboardMap[SDLK_S] = Button::Switch;
            this->keyboardMap[SDLK_TAB] = Button::Menu;
            this->keyboardMap[SDLK_DELETE] = Button::Scroll;
        }
    }
}
