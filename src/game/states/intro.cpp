#include "intro.hpp"

namespace Game {
    namespace States {
        const std::string Intro::logChannel = "states.intro";

        Intro::Intro() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Intro::logChannel);
            this->pop = false;
            this->LoadResources("resources/asset_lists/intro_textures.json", "resources/asset_lists/intro_sounds.json");
            this->LoadLuaState("scripts/states/intro.lua");
        }

        Intro::~Intro() {
        }

        void Intro::HandleEvent(const SDL_Event& event) {
        }

        Transition Intro::Update(const float deltaTime) {
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
            case GameStateType::main_menu:
                return Transition::Push(std::make_shared<MainMenu>());
            default:
                return Transition::None();
            }
        }

        std::string Intro::ProcessInput(const Input::Button key) {
            return "";
        }

        GameStateType Intro::ProcessInput() {
            if (Services::Locator::InputService()->GetCurrentInputState().confirmPressed) {
                return GameStateType::main_menu;
            }

            return GameStateType::intro;
        }

        void Intro::Render() {
            sol::protected_function render = (*this->luaState.get())["render"];

            if (render.valid()) {
                sol::protected_function_result result = render();

                if (!result.valid()) {
                    sol::error err = result;
                    this->logger->error() << "Error calling Lua render function: " << err.what();
                }
            }
        }

        void Intro::LoadLuaState(const std::string& scriptFilePath) {
            Base::LoadLuaState(scriptFilePath);

            this->luaState->open_libraries(sol::lib::math, sol::lib::os);

            Objects::Text::LuaInterface::Bind(this->luaState);
            Intro::LuaInterface::Bind(this->luaState);

            this->luaState->set("intro_state", this);

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

        void Intro::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table states = (*luaState.get())["states"].get_or_create<sol::table>(sol::new_table());

            states.new_usertype<Intro>("Intro",
                                       sol::no_constructor,
                                       "pop", &Intro::Pop,
                                       "get_texture", &Intro::GetTexture
                                       );
        }
    }
}
