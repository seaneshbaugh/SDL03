#ifndef SDL03_Game_Objects_Characters_Monster
#define SDL03_Game_Objects_Characters_Monster

#include "base.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            class Monster : public Base {
            public:
                Monster();
                ~Monster();

                class LuaInterface {
                public:
                    static void Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext);
                };
            };
        }
    }
}

#endif
