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
            std::string ProcessInput(int);
            void Render();

        private:
            static const std::string logChannel;

            void LoadLuaContext(const std::string& scriptFile);
            void EnableRawInput();
            void DisableRawInput();

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext);
            };
        };
    }
}

#endif
