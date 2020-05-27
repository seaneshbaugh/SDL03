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
            std::shared_ptr<Base> Update(const int key);
            std::shared_ptr<Base> Update(const SDL_Event& event);
            std::string ProcessInput(const int key);
            void Render();

        private:
            static const std::string logChannel;

            void EnableRawInput();
            void DisableRawInput();
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
