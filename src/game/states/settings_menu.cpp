#include "settings_menu.hpp"

namespace Game {
    namespace States {
        const std::string SettingsMenu::logChannel = "states.settings_menu";

        SettingsMenu::SettingsMenu() {
            this->logger = Services::Locator::LoggerService()->GetLogger(SettingsMenu::logChannel);
            this->pop = false;
            this->acceptRawInput = false;
            this->LoadResources("resources/asset_lits/settings_menu_textures.json", "resources/asset_lits/settings_menu_sounds.json");
            this->LoadLuaContext("scripts/states/settings_menu.lua");
        }

        SettingsMenu::~SettingsMenu() {
        }

        // TODO: Refactor as much of this as possible into base class.
        void SettingsMenu::LoadLuaContext(const std::string& scriptFileName) {
            this->luaContext = std::make_shared<LuaIntf::LuaContext>();

            Objects::Text::LuaInterface::BindOld(this->luaContext);
            Objects::Image::LuaInterface::BindOld(this->luaContext);
            SettingsMenu::LuaInterface::Bind(this->luaContext);

            LuaIntf::Lua::setGlobal(this->luaContext->state(), "raw_settings_menu_state", this->shared_from_this());

            this->logger->debug() << "Loading \"" << scriptFileName << "\".";

            this->luaContext->doFile(scriptFileName.c_str());

            this->logger->debug() << "Loaded \"" << scriptFileName << "\".";

            LuaIntf::LuaRef initialize(this->luaContext->state(), "initialize");

            // TODO: Handle errors?
            initialize();
        }

        std::shared_ptr<Base> SettingsMenu::Update(const int key) {
            std::string nextState = "";

            if (key != static_cast<int>(InputKey::NO_KEY)) {
                nextState = this->ProcessInput(key);
            }

            LuaIntf::LuaRef update(this->luaContext->state(), "update");

            update();

            if (this->pop) {
                return nullptr;
            }

            return this->shared_from_this();
        }

        std::shared_ptr<Base> SettingsMenu::Update(const SDL_Event& event) {
            return this->Update(event.key.keysym.sym);
        }

        std::string SettingsMenu::ProcessInput(int key) {
            LuaIntf::LuaRef processInput(this->luaContext->state(), "process_input");

            std::string result = processInput.call<std::string>(key);

            return result;
        }

        void SettingsMenu::Render() {
            LuaIntf::LuaRef render(this->luaContext->state(), "render");

            render();
        }

        void SettingsMenu::EnableRawInput() {
            this->acceptRawInput = true;
        }

        void SettingsMenu::DisableRawInput() {
            this->acceptRawInput = false;
        }

        void SettingsMenu::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
//            LuaIntf::LuaBinding(luaContext->state())
//            .beginModule("states")
//                .beginClass<SettingsMenu>("SettingsMenu")
//                    .addConstructor(LUA_SP(std::shared_ptr<SettingsMenu>), LUA_ARGS())
//                    .addFunction("pop", &SettingsMenu::Pop)
//                    //.addFunction("process_input", LUA_FN(std::string, SettingsMenu::ProcessInput, int))
//                    .addFunction("process_input", static_cast<std::string(Game::States::SettingsMenu::*)(const int)>(&SettingsMenu::ProcessInput))
//                    .addFunction("render", &SettingsMenu::Render)
//                    .addFunction("get_texture", &SettingsMenu::GetTexture, LUA_ARGS(LuaIntf::_opt<std::string>))
//                    .addFunction("enable_raw_input", &SettingsMenu::EnableRawInput)
//                    .addFunction("disable_raw_input", &SettingsMenu::DisableRawInput)
//                .endClass()
//            .endModule();
        }
    }
}
