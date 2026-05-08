#ifndef SDL03_Game_States_SettingsMenu
#define SDL03_Game_States_SettingsMenu

#include "base.hpp"
#include "../objects/image.hpp"

namespace Game {
    namespace States {
        class SettingsMenu : public Base {
        public:
            SettingsMenu();
            ~SettingsMenu();
            void HandleEvent(const SDL_Event& event);
            std::shared_ptr<Base> Update();
            void Render();

        private:
            static const std::string logChannel;

            std::string ProcessInput(const InputKey key);
            void LoadLuaState(const std::string& scriptFilePath);

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<sol::state> luaState);
            };
        };
    }
}

#endif
