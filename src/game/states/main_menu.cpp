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
            //Input::Button key = Services::Locator::InputService()->GetInputButton(event);

            //if (key != Input::Button::None) {
            //    this->ProcessInput(key);
            //}
        }

        Transition MainMenu::Update(const float deltaTime) {
            GameStateType nextState = this->ProcessInput();

            sol::protected_function update = (*this->luaState.get())["update"];

            if (update.valid()) {
                sol::protected_function_result result = update(deltaTime);

                if (!result.valid()) {
                    sol::error err = result;
                    this->logger->error() << "Error calling Lua update function: " << err.what();
                }
            }

            if (this->pop) {
                return Transition::Pop();
            }

            switch (nextState) {
            case GameStateType::new_game:
                return Transition::Replace(Services::Locator::WorldService()->NewGame());
            //case GameStateType::settings_menu:
            //    return Transition::Replace(std::make_shared<SettingsMenu>());
            // case GameStateType::load_game_menu:
            //     return Transition::Replace(std::make_shared<LoadGameMenu>());
            default:
                return Transition::None();
            }
        }

        std::string MainMenu::ProcessInput(const Input::Button key) {
            return "";
        }

        GameStateType MainMenu::ProcessInput() {
            sol::protected_function processInput = (*this->luaState.get())["process_input"];

            if (processInput.valid()) {
                sol::protected_function_result result = processInput(Services::Locator::InputService()->GetCurrentInputState());

                if (!result.valid()) {
                    sol::error err = result;
                    this->logger->error() << "Error calling Lua process_input function: " << err.what();

                    return GameStateType::main_menu;
                }

                std::string nextState = result.get<std::string>();

                return StateNameToEnum(nextState);
            }

            return GameStateType::main_menu;
        }

        void MainMenu::Render() {
            sol::protected_function render = (*this->luaState.get())["render"];

            if (render.valid()) {
                sol::protected_function_result result = render();

                if (!result.valid()) {
                    sol::error err = result;
                    this->logger->error() << "Error calling Lua render function: " << err.what();
                }
            }
        }

        void MainMenu::LoadLuaState(const std::string& scriptFilePath) {
            Base::LoadLuaState(scriptFilePath);

            Input::InputState::LuaInterface::Bind(this->luaState);
            Objects::Text::LuaInterface::Bind(this->luaState);
            Objects::Image::LuaInterface::Bind(this->luaState);
            MainMenu::LuaInterface::Bind(this->luaState);

            this->luaState->set("main_menu_state", this);

            this->logger->debug() << "Loading \"" << scriptFilePath << "\".";

            this->luaState->script_file(scriptFilePath);

            this->logger->debug() << "Loaded \"" << scriptFilePath << "\".";

            sol::protected_function initialize = (*this->luaState.get())["initialize"];

            if (initialize.valid()) {
                sol::protected_function_result result = initialize();

                if (!result.valid()) {
                    sol::error err = result;
                    this->logger->error() << "Error calling Lua initialize function: " << err.what();
                }
            }
        }

        void MainMenu::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table states = (*luaState.get())["states"].get_or_create<sol::table>(sol::new_table());

            states.new_usertype<MainMenu>("MainMenu",
                                          sol::no_constructor,
                                          "pop", &MainMenu::Pop,
                                          "get_texture", &MainMenu::GetTexture
                                          );
        }
    }
}
