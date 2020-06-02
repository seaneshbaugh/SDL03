#ifndef SDL03_Game_States_SaveGameMenu
#define SDL03_Game_States_SaveGameMenu

#include "base.hpp"
#include "../objects/image.hpp"

namespace Game {
    namespace States {
        class SaveGameMenu : public Base {
        public:
            SaveGameMenu();
            ~SaveGameMenu();
            std::shared_ptr<Base> Update(const int key);
            std::shared_ptr<Base> Update(const SDL_Event& event);
            std::string ProcessInput(const int key);
            void Render();
            void SaveGame(const unsigned int slot);

        private:
            static const std::string logChannel;

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
