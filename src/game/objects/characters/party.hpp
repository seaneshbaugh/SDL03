#ifndef SDL03_Game_Objects_Characters_Party
#define SDL03_Game_Objects_Characters_Party

#include <vector>

#include "base.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            class Party : public Base {
            public:
                std::vector<std::shared_ptr<Base>> characters;

                Party();
                ~Party();

                class LuaInterface {
                public:
                    void Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext);
                };
            };
        }
    }
}

#endif
