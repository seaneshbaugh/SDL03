#include "intro.hpp"

namespace Game {
    namespace States {
        const std::string Intro::logChannel = "states.intro";

        Intro::Intro() {
            this->logger = Services::Locator::LoggerService()->GetLogger(Intro::logChannel);
            this->pop = false;
            this->acceptRawInput = false;
            this->LoadResources("resources/asset_lists/intro_textures.json", "resources/asset_lists/intro_sounds.json");
            this->LoadLuaContext("scripts/states/intro.lua");
        }

        Intro::~Intro() {
        }

        // TODO: Refactor as much of this as possible into base class.
        void Intro::LoadLuaContext(const std::string& scriptFileName) {
            this->luaContext = std::make_shared<LuaIntf::LuaContext>();

            Objects::Text::LuaInterface::Bind(this->luaContext);
            Intro::LuaInterface::Bind(this->luaContext);

            LuaIntf::Lua::setGlobal(this->luaContext->state(), "intro_state", this);

            this->logger->debug() << "Loading \"" << scriptFileName << "\".";

            this->luaContext->doFile(scriptFileName.c_str());

            this->logger->debug() << "Loaded \"" << scriptFileName << "\".";

            LuaIntf::LuaRef initialize(this->luaContext->state(), "initialize");

            // TODO: Handle errors?
            initialize();
        }

        std::shared_ptr<Base> Intro::Update(const int key) {
            std::string nextState = "intro";

            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                nextState = this->ProcessInput(key);
            }

            LuaIntf::LuaRef update(this->luaContext->state(), "update");

            update();

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
        // any keyboard input just immediately causes a transition to the main menu state.
        // Honestly it doesn't even make sense to call the Lua function at all, but I'm
        // leaving it in for consistency's sake.
        std::string Intro::ProcessInput(const int key) {
            LuaIntf::LuaRef processInput(this->luaContext->state(), "process_input");

            std::string result = processInput.call<std::string>(key);

            return result;
        }

        void Intro::Render() {
            LuaIntf::LuaRef render(this->luaContext->state(), "render");

            render();
        }

        void Intro::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
            LuaIntf::LuaBinding(luaContext->state())
            .beginModule("states")
                .beginClass<Intro>("Intro")
                    .addConstructor(LUA_SP(std::shared_ptr<Intro>), LUA_ARGS())
                    .addFunction("pop", &Intro::Pop)
                    .addFunction("process_input", static_cast<std::string(Game::States::Intro::*)(const int)>(&Intro::ProcessInput))
                    .addFunction("render", &Intro::Render)
                    .addFunction("get_texture", &Intro::GetTexture, LUA_ARGS(LuaIntf::_opt<std::string>))
                .endClass()
            .endModule();
        }
    }
}
