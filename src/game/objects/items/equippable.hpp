#ifndef SDL03_Game_Objects_Items_Equippable
#define SDL03_Game_Objects_Items_Equippable

#include "base.hpp"

namespace Game {
    namespace Objects {
        namespace Items {
            class Equippable : public Base {
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
