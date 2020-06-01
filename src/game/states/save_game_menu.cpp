#include "save_game_menu.hpp"

namespace Game {
    namespace States {
        const std::string SaveGameMenu::logChannel = "states.save_game_menu";

        SaveGameMenu::SaveGameMenu() {
            this->logger = Services::Locator::LoggerService()->GetLogger(SaveGameMenu::logChannel);
            this->pop = false;
            this->acceptRawInput = false;

            this->LoadResources("resources/asset_lists/save_game_menu_textures.json", "resources/asset_lists/save_game_menu_sounds.json");

            this->LoadLuaState("scripts/states/save_game_menu.lua");
        }

        SaveGameMenu::~SaveGameMenu() {
        }

        std::shared_ptr<Base> SaveGameMenu::Update(const int key) {
            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                this->ProcessInput(key);
            }

            std::string nextState = (*this->luaState.get())["update"]();

            if (this->pop) {
                return nullptr;
            }

            return this->shared_from_this();
        }

        std::shared_ptr<Base> SaveGameMenu::Update(const SDL_Event& event) {
            return this->Update(event.key.keysym.sym);
        }

        std::string SaveGameMenu::ProcessInput(const int key) {
            std::string result = (*this->luaState.get())["process_input"](key);

            return result;
        }

        void SaveGameMenu::Render() {
            (*this->luaState.get())["render"]();
        }

        void SaveGameMenu::LoadLuaState(const std::string& scriptFilePath) {
            Base::LoadLuaState(scriptFilePath);

            this->luaState->open_libraries(sol::lib::string);

            Objects::Text::LuaInterface::Bind(this->luaState);
            Objects::Image::LuaInterface::Bind(this->luaState);
            SaveGameMenu::LuaInterface::Bind(this->luaState);

            this->luaState->set("save_game_menu_state", this);

            this->logger->debug() << "Loading \"" << scriptFilePath << "\".";

            this->luaState->script_file(scriptFilePath);

            this->logger->debug() << "Loaded \"" << scriptFilePath << "\".";

            // TODO: Handle errors?
            (*this->luaState.get())["initialize"]();
        }

        void SaveGameMenu::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table states = (*luaState.get())["states"].get_or_create<sol::table>(sol::new_table());

            states.new_usertype<SaveGameMenu>("SaveGameMenu",
                                              sol::no_constructor,
                                              "pop", &SaveGameMenu::Pop,
                                              "processInput", static_cast<std::string (SaveGameMenu::*)(const int)>(&SaveGameMenu::ProcessInput)
                                              );
        }
    }
}
