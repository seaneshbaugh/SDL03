#include "input_mapper.hpp"

namespace Game {
    namespace Input {
        const std::string InputMapper::logChannel = "input";

        InputMapper::InputMapper() {
            this->logger = Services::Locator::LoggerService()->GetLogger(InputMapper::logChannel);
        }

        InputMapper::InputMapper(const std::map<int, Button>& buttons) : InputMapper() {
            this->MapButtons(buttons);
        }

        InputMapper::~InputMapper() {
        }

        std::map<int, Button>::size_type InputMapper::MapButtons(const std::map<int, Button>& buttons) {
            for (auto it = buttons.begin(); it != buttons.end(); ++it) {
                this->SetInputMapKey(it->first, it->second);
            }

            return buttons.size();
        }

        Button InputMapper::SetInputMapKey(const int& rawKeyValue, const Button& inputValue) {
            for (auto it = this->inputMap.begin(); it != this->inputMap.end();) {
                if (it->second == inputValue) {
                    this->inputMap.erase(it++);
                } else {
                    ++it;
                }
            }

            this->inputMap[rawKeyValue] = inputValue;

            return inputValue;
        }

        Button InputMapper::GetInputButton(const SDL_Event& event) {
            try {
                if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                    return this->inputMap.at(event.key.key);
                } else {
                    return Button::None;
                }
            } catch (const std::out_of_range& exception) {
                return Button::None;
            }
        }

        // TODO: Figure out a better place for this.
        void InputMapper::SetDefaultInputMap() {
            this->inputMap[SDLK_UP] = Button::Up;
            this->inputMap[SDLK_DOWN] = Button::Down;
            this->inputMap[SDLK_LEFT] = Button::Left;
            this->inputMap[SDLK_RIGHT] = Button::Right;
            this->inputMap[SDLK_Z] = Button::Confirm;
            this->inputMap[SDLK_X] = Button::Cancel;
            this->inputMap[SDLK_A] = Button::Skip;
            this->inputMap[SDLK_S] = Button::Switch;
            this->inputMap[SDLK_TAB] = Button::Menu;
            this->inputMap[SDLK_DELETE] = Button::Scroll;
        }
    }
}
