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
            void HandleEvent(const SDL_Event& event);
            Transition Update(const double deltaTime);
            void Render();
            void SaveGame(const unsigned int slot);

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
