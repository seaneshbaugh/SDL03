#include "input_mapper.hpp"

namespace Game {
    const std::string InputMapper::logChannel = "input";

    InputMapper::InputMapper() {
        this->logger = Services::Locator::LoggerService()->GetLogger(InputMapper::logChannel);
    }

    InputMapper::InputMapper(const std::map<int, InputKey>& keys) : InputMapper() {
        this->MapKeys(keys);
    }

    InputMapper::~InputMapper() {
    }

    std::map<int, InputKey>::size_type InputMapper::MapKeys(const std::map<int, InputKey>& keys) {
        for (auto it = keys.begin(); it != keys.end(); ++it) {
            this->SetInputMapKey(it->first, it->second);
        }

        return keys.size();
    }

    InputKey InputMapper::SetInputMapKey(const int& rawKeyValue, const InputKey& inputValue) {
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

    InputKey InputMapper::GetInputMapKey(const SDL_Event& event) {
        try {
            if (event.type == SDL_KEYDOWN) {
                return this->inputMap.at(event.key.keysym.sym);
            } else {
                return InputKey::NO_KEY;
            }
        } catch (const std::out_of_range& exception) {
            return InputKey::NO_KEY;
        }
    }

    // TODO: Figure out a better place for this.
    void InputMapper::SetDefaultInputMap() {
        this->inputMap[SDLK_UP] = InputKey::UP_KEY;
        this->inputMap[SDLK_DOWN] = InputKey::DOWN_KEY;
        this->inputMap[SDLK_LEFT] = InputKey::LEFT_KEY;
        this->inputMap[SDLK_RIGHT] = InputKey::RIGHT_KEY;
        this->inputMap[SDLK_z] = InputKey::CONFIRM_KEY;
        this->inputMap[SDLK_x] = InputKey::CANCEL_KEY;
        this->inputMap[SDLK_TAB] = InputKey::MENU_KEY;
    }
}
