#include "settings_menu.hpp"

namespace Game {
    namespace States {
        const std::string SettingsMenu::logChannel = "states.settings_menu";

        SettingsMenu::SettingsMenu() {
            this->logger = Services::Locator::LoggerService()->GetLogger(SettingsMenu::logChannel);
            this->pop = false;

            this->LoadResources("resources/asset_lists/settings_menu_textures.json", "resources/asset_lists/settings_menu_sounds.json");

            this->LoadLuaState("scripts/states/settings_menu.lua");
        }

        SettingsMenu::~SettingsMenu() {
        }

        void SettingsMenu::HandleEvent(const SDL_Event& event) {
            InputKey key = Services::Locator::InputService()->GetInputMapKey(event);

            if (key != InputKey::NO_KEY) {
                this->ProcessInput(key);
            }
        }

        Transition SettingsMenu::Update(const double deltaTime) {
            std::string nextState = (*this->luaState.get())["update"](deltaTime);

            if (this->pop) {
                return Transition::Pop();
            }

            return Transition::None();
        }


        std::string SettingsMenu::ProcessInput(const InputKey key) {
            std::string result = (*this->luaState.get())["process_input"](static_cast<int>(key));

            return result;
        }

        void SettingsMenu::Render() {
            (*this->luaState.get())["render"]();
        }

        void SettingsMenu::LoadLuaState(const std::string& scriptFilePath) {
            Base::LoadLuaState(scriptFilePath);

            this->luaState->open_libraries(sol::lib::string);

            Objects::Text::LuaInterface::Bind(this->luaState);
            Objects::Image::LuaInterface::Bind(this->luaState);
            SettingsMenu::LuaInterface::Bind(this->luaState);

            this->luaState->set("settings_menu_state", this);

            this->logger->debug() << "Loading \"" << scriptFilePath << "\".";

            this->luaState->script_file(scriptFilePath);

            this->logger->debug() << "Loaded \"" << scriptFilePath << "\".";

            // TODO: Handle errors?
            (*this->luaState.get())["initialize"]();
        }

        void SettingsMenu::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table states = (*luaState.get())["states"].get_or_create<sol::table>(sol::new_table());

            states.new_usertype<SettingsMenu>("SettingsMenu",
                                              sol::no_constructor,
                                              "pop", &SettingsMenu::Pop,
                                              "processInput", static_cast<std::string (SettingsMenu::*)(const InputKey)>(&SettingsMenu::ProcessInput)
                                              );
        }
    }
}
