#include "settings_menu.hpp"

namespace Game {
    namespace States {
        const std::string SettingsMenu::logChannel = "states.settings_menu";

        SettingsMenu::SettingsMenu() {
            this->logger = Services::Locator::LoggerService()->GetLogger(SettingsMenu::logChannel);
            this->pop = false;
            this->acceptRawInput = false;

            this->LoadResources("resources/asset_lists/settings_menu_textures.json", "resources/asset_lists/settings_menu_sounds.json");

            this->LoadLuaState("scripts/states/settings_menu.lua");
        }

        SettingsMenu::~SettingsMenu() {
        }

        std::shared_ptr<Base> SettingsMenu::Update(const int key) {
            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                this->ProcessInput(key);
            }

            std::string nextState = (*this->luaState.get())["update"]();

            if (this->pop) {
                return nullptr;
            }

            return this->shared_from_this();
        }

        std::shared_ptr<Base> SettingsMenu::Update(const SDL_Event& event) {
            return this->Update(event.key.keysym.sym);
        }

        std::string SettingsMenu::ProcessInput(const int key) {
            std::string result = (*this->luaState.get())["process_input"](key);

            return result;
        }

        void SettingsMenu::Render() {
            (*this->luaState.get())["render"]();
        }

        void SettingsMenu::EnableRawInput() {
            this->acceptRawInput = true;
        }

        void SettingsMenu::DisableRawInput() {
            this->acceptRawInput = false;
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
                                              "processInput", static_cast<std::string (SettingsMenu::*)(const int)>(&SettingsMenu::ProcessInput),
                                              "disableRawInput", &SettingsMenu::DisableRawInput,
                                              "enableRawInput", &SettingsMenu::EnableRawInput
                                              );
        }
    }
}
