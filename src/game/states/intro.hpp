#ifndef SDL03_Game_States_Intro
#define SDL03_Game_States_Intro

#include "../../../lib/LoggerCpp/include/LoggerCpp/LoggerCpp.h"

#include "base.hpp"
#include "main_menu.hpp"

namespace Game {
    namespace States {
        class Intro : public Base {
        public:
            Intro();
            ~Intro();
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
