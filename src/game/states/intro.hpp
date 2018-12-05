#ifndef SDL03_Game_States_Intro
#define SDL03_Game_States_Intro

#include "LoggerCpp.h"

#include "base.hpp"
#include "main_menu.hpp"

namespace Game {
    namespace States {
        class Intro : public Base {
        public:
            Intro();
            ~Intro();
            std::shared_ptr<Base> Update(const int key);
            std::shared_ptr<Base> Update(const SDL_Event& event);
            std::string ProcessInput(const int key);
            void Render();

        private:
            static const std::string logChannel;

            void LoadLuaContext(const std::string& scriptFile);

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext);
            };
        };
    }
}

#endif
