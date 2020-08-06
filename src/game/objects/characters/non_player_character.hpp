#ifndef SDL03_Game_Objects_Characters_NonPlayerCharacter
#define SDL03_Game_Objects_Characters_NonPlayerCharacter

#include "base.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            class NonPlayerCharacter : public Base {
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
