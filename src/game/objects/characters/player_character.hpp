#ifndef SDL03_Game_Objects_Characters_PlayerCharacter
#define SDL03_Game_Objects_Characters_PlayerCharacter

#include "base.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            class PlayerCharacter : public Base {
            public:
                static const unsigned long long int EXPERIENCE_REQUIRED_FOR_LEVEL[99];

            public:
                class LuaInterface {
                public:
                    static void Bind(std::shared_ptr<sol::state> luaState);
                };
            };
        }
    }
}

#endif
