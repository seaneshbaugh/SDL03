#ifndef SDL03_Game_States_MainMenu
#define SDL03_Game_States_MainMenu

#include "base.hpp"
#include "map.hpp"
#include "settings_menu.hpp"
#include "../objects/image.hpp"

namespace Game {
    namespace States {
        class MainMenu : public Base {
        public:
            MainMenu();
            ~MainMenu();
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
