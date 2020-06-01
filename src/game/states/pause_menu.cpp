#include "pause_menu.hpp"

namespace Game {
    namespace States {
        const std::string PauseMenu::logChannel = "states.pause_menu";

        PauseMenu::PauseMenu() {
            this->logger = Services::Locator::LoggerService()->GetLogger(PauseMenu::logChannel);
            this->pop = false;
            this->acceptRawInput = false;

            // TODO: Figure out a way to either pre-load these resources or otherwise
            // keep them around after this state is popped. Parsing a JSON file every
            // time the player opens the pause menu is not a good idea.
            // One possibility is having a list of pre-loadable resource lists that
            // are loaded once a new game is started or once a save is loaded. These
            // lists can be loaded, parsed, and kept in memory and then referenced
            // when needed. I'm not 100% certain what this will look like, but I think
            // I need to have some mechanism in place to pre-load resource lists and
            // ensure they stick around. Some things are likely to be used so often
            // that loading them every time a state transition happens makes no sense.
            // For now this is okay since the process is still pretty fast.
            this->LoadResources("resources/asset_lists/pause_menu_textures.json", "resources/asset_lists/pause_menu_sounds.json");

            this->LoadLuaState("scripts/states/pause_menu.lua");
        }

        PauseMenu::~PauseMenu() {
        }

        std::shared_ptr<Base> PauseMenu::Update(const int key) {
            if (static_cast<InputKey>(key) != InputKey::NO_KEY) {
                this->ProcessInput(key);
            }

            std::string nextState = (*this->luaState.get())["update"]();

            if (this->pop) {
                return nullptr;
            }

            switch (StateNameToEnum(nextState)) {
                case GameStateType::save_game_menu:
                    return std::make_shared<SaveGameMenu>();
                default:
                    return this->shared_from_this();
            }
        }

        std::shared_ptr<Base> PauseMenu::Update(const SDL_Event& event) {
            return this->Update(event.key.keysym.sym);
        }

        std::string PauseMenu::ProcessInput(const int key) {
            std::string result = (*this->luaState.get())["process_input"](key);

            return result;
        }

        void PauseMenu::Render() {
            (*this->luaState.get())["render"]();
        }

        std::tuple<unsigned int, unsigned int, unsigned int> PauseMenu::GetClockTime() {
            return Services::Locator::TimeService()->GetClockTime();
        }

        void PauseMenu::LoadLuaState(const std::string& scriptFilePath) {
            Base::LoadLuaState(scriptFilePath);

            this->luaState->open_libraries(sol::lib::string);

            Objects::Text::LuaInterface::Bind(this->luaState);
            Objects::Image::LuaInterface::Bind(this->luaState);
            PauseMenu::LuaInterface::Bind(this->luaState);

            this->luaState->set("pause_menu_state", this);

            this->logger->debug() << "Loading \"" << scriptFilePath << "\".";

            this->luaState->script_file(scriptFilePath);

            this->logger->debug() << "Loaded \"" << scriptFilePath << "\".";

            // TODO: Handle errors?
            (*this->luaState.get())["initialize"]();
        }

        void PauseMenu::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table states = (*luaState.get())["states"].get_or_create<sol::table>(sol::new_table());

            states.new_usertype<PauseMenu>("PauseMenu",
                                           sol::no_constructor,
                                           "pop", &PauseMenu::Pop,
                                           "processInput", static_cast<std::string (PauseMenu::*)(const int)>(&PauseMenu::ProcessInput),
                                           "getClockTime", &PauseMenu::GetClockTime
                                           );
        }
    }
}
