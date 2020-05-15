#include "intro.hpp"

namespace Game {
    namespace States {
        const std::string Intro::logChannel = "states.intro";

        Intro::Intro() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Intro::logChannel);
            this->pop = false;
            this->acceptRawInput = false;
            this->LoadResources("resources/asset_lists/intro_textures.json", "resources/asset_lists/intro_sounds.json");
            this->LoadLuaState("scripts/states/intro.lua");
        }

        Intro::~Intro() {
        }

        std::shared_ptr<Base> Intro::Update(const int key) {
            std::string nextState = "intro";

            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                nextState = this->ProcessInput(key);
            }

            (*this->luaState.get())["update"]();

            if (this->pop) {
                return nullptr;
            }

            switch(StateNameToEnum(nextState)) {
                case GameStateType::main_menu:
                    return std::make_shared<MainMenu>();
                default:
                    return this->shared_from_this();
            }
        }

        std::shared_ptr<Base> Intro::Update(const SDL_Event& event) {
            return this->Update(event.key.keysym.sym);
        }

        // For the intro state the Lua process_input function does nothing. This is because
        // any keyboard input with a valid input mapper binding just immediately causes a
        // transition to the main menu state. Honestly it doesn't even make sense to call
        // the Lua function at all, but I'm leaving it in for consistency's sake.
        std::string Intro::ProcessInput(const int key) {
            std::string result = (*this->luaState.get())["process_input"](key);

            return result;
        }

        void Intro::Render() {
            (*this->luaState.get())["render"]();
        }

        // TODO: Remove this once Base state class no longer defines it.
        void Intro::LoadLuaContext(const std::string& scriptFilePath) {
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

            (*this->luaState.get())["initialize"]();
        }

        void Intro::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table states = (*luaState.get())["states"].get_or_create<sol::table>(sol::new_table());

            states.new_usertype<Intro>("Intro",
                                       sol::no_constructor,
                                       "pop", &Intro::Pop,
                                       "process_input", static_cast<std::string(Intro::*)(const int)>(&Intro::ProcessInput),
                                       "render", &Intro::Render,
                                       "get_texture", &Intro::GetTexture
                                       );

        }
    }
}
