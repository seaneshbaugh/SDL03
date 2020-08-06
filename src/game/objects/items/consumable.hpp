#ifndef SDL03_Game_Objects_Items_Consumable
#define SDL03_Game_Objects_Items_Consumable

#include "base.hpp"

namespace Game {
    namespace Objects {
        namespace Items {
            class Consumable : public Base {
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
