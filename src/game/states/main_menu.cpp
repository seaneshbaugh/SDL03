#include "main_menu.hpp"

namespace Game {
    namespace States {
        const std::string MainMenu::logChannel = "states.main_menu";

        MainMenu::MainMenu() {
            this->logger = Services::Locator::LoggerService()->GetLogger(MainMenu::logChannel);
            this->pop = false;
            this->LoadResources("resources/asset_lists/main_menu_textures.json", "resources/asset_lists/main_menu_sounds.json");
            this->LoadLuaState("scripts/states/main_menu.lua");
        }

        MainMenu::~MainMenu() {
        }

        void MainMenu::HandleEvent(const SDL_Event& event) {
            InputKey key = Services::Locator::InputService()->GetInputMapKey(event);

            if (key != InputKey::NO_KEY) {
                this->ProcessInput(key);
            }
        }

        Transition MainMenu::Update(const double deltaTime) {
            std::string nextState = (*this->luaState.get())["update"](deltaTime);

            if (this->pop) {
                return Transition::Pop();
            }

            switch (StateNameToEnum(nextState)) {
            case GameStateType::new_game:
                return Transition::Push(Services::Locator::WorldService()->NewGame());
            case GameStateType::settings_menu:
                return Transition::Push(std::make_shared<SettingsMenu>());
            // case GameStateType::load_game_menu:
            //     return Transition::Push(std::make_shared<LoadGameMenu>());
            default:
                return Transition::None();
            }
        }

        std::string MainMenu::ProcessInput(const InputKey key) {
            std::string result = (*this->luaState.get())["process_input"](static_cast<int>(key));

            return result;
        }

        void MainMenu::Render() {
            (*this->luaState.get())["render"]();
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
                                          "process_input", static_cast<std::string (MainMenu::*)(const InputKey)>(&MainMenu::ProcessInput),
                                          "render", &MainMenu::Render
                                          );
        }
    }
}
