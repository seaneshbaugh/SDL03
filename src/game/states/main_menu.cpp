#include "main_menu.hpp"

namespace Game {
    namespace States {
        const std::string MainMenu::logChannel = "states.main_menu";

        MainMenu::MainMenu() {
            this->logger = Services::Locator::LoggerService()->GetLogger(MainMenu::logChannel);
            this->pop = false;
            this->acceptRawInput = false;
            this->LoadResources("resources/asset_lists/main_menu_textures.json", "resources/asset_lists/main_menu_sounds.json");
            this->LoadLuaState("scripts/states/main_menu.lua");
        }

        MainMenu::~MainMenu() {
        }

        std::shared_ptr<Base> MainMenu::Update(const int key) {
            std::string nextState = "main_menu";

            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                nextState = this->ProcessInput(key);
            }

            (*this->luaState.get())["update"]();

            if (this->pop) {
                return nullptr;
            }

            switch(StateNameToEnum(nextState)) {
                case GameStateType::new_game:
                    return Services::Locator::WorldService()->NewGame();
                case GameStateType::settings_menu:
                    return std::make_shared<SettingsMenu>();
//                case GameStateType::load_game_menu:
//                    return std::make_shared<LoadGameMenu>();
                default:
                    return this->shared_from_this();
            }
        }

        std::shared_ptr<Base> MainMenu::Update(const SDL_Event& event) {
            return this->Update(event.key.keysym.sym);
        }

        std::string MainMenu::ProcessInput(const int key) {
            std::string result = (*this->luaState.get())["process_input"](key);

            return result;
        }

        void MainMenu::Render() {
            (*this->luaState.get())["render"]();
        }

        // TODO: Remove this once Base state class no longer defines it.
        void MainMenu::LoadLuaContext(const std::string& scriptFilePath) {
        }

        void MainMenu::LoadLuaState(const std::string& scriptFilePath) {
            Base::LoadLuaState(scriptFilePath);

            Objects::Text::LuaInterface::Bind(this->luaState);
            Objects::Image::LuaInterface::Bind(this->luaState);
            MainMenu::LuaInterface::Bind(this->luaState);

            this->luaState->set("main_menu_state", this);

            this->logger->debug() << "Loading \"" << scriptFilePath << "\".";

            this->luaState->script_file(scriptFilePath);

            this->logger->debug() << "Loaded \"" << scriptFilePath << "\".";

            // TODO: Handle errors?
            (*this->luaState.get())["initialize"]();
        }

        void MainMenu::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table states = (*luaState.get())["states"].get_or_create<sol::table>(sol::new_table());

            states.new_usertype<MainMenu>("MainMenu",
                                          sol::no_constructor,
                                          "pop", &MainMenu::Pop,
                                          "process_input", static_cast<std::string (MainMenu::*)(const int)>(&MainMenu::ProcessInput),
                                          "render", &MainMenu::Render
                                          );
        }
    }
}
