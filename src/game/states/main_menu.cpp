#include "main_menu.hpp"

namespace Game {
    namespace States {
        const std::string MainMenu::logChannel = "states.main_menu";

        MainMenu::MainMenu() {
            this->logger = Services::Locator::LoggerService()->GetLogger(MainMenu::logChannel);
            this->pop = false;
            this->acceptRawInput = false;
            this->LoadResources("resources/asset_lists/main_menu_textures.json", "resources/asset_lists/main_menu_sounds.json");
            this->LoadLuaContext("scripts/states/main_menu.lua");
        }

        MainMenu::~MainMenu() {
        }

        void MainMenu::LoadLuaContext(const std::string& scriptFileName) {
            this->luaContext = std::make_shared<LuaIntf::LuaContext>();

            Objects::Text::LuaInterface::Bind(this->luaContext);
            Objects::Image::LuaInterface::Bind(this->luaContext);
            MainMenu::LuaInterface::Bind(this->luaContext);

            LuaIntf::Lua::setGlobal(this->luaContext->state(), "main_menu_state", this);

            this->logger->debug() << "Loading \"" << scriptFileName << "\".";

            this->luaContext->doFile(scriptFileName.c_str());

            this->logger->debug() << "Loaded \"" << scriptFileName << "\".";

            LuaIntf::LuaRef initialize(this->luaContext->state(), "initialize");

            // TODO: Handle errors?
            initialize();
        }

        std::shared_ptr<Base> MainMenu::Update(const int key) {
            std::string nextState = "main_menu";

            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                nextState = this->ProcessInput(key);
            }

            LuaIntf::LuaRef update(this->luaContext->state(), "update");

            update();

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
            LuaIntf::LuaRef processInput(this->luaContext->state(), "process_input");

            std::string result = processInput.call<std::string>(key);

            return result;
        }

        void MainMenu::Render() {
            LuaIntf::LuaRef render(this->luaContext->state(), "render");

            render();
        }

        void MainMenu::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
            LuaIntf::LuaBinding(luaContext->state())
            .beginModule("states")
                .beginClass<MainMenu>("MainMenu")
                    .addConstructor(LUA_SP(std::shared_ptr<MainMenu>), LUA_ARGS())
                    .addFunction("pop", &MainMenu::Pop)
                    .addFunction("process_input", static_cast<std::string(Game::States::MainMenu::*)(const int)>(&MainMenu::ProcessInput))
                    .addFunction("render", &MainMenu::Render)
                .endClass()
            .endModule();
        }
    }
}
