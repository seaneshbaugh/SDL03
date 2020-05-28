#ifndef SDL03_Game_Objects_Characters_Party
#define SDL03_Game_Objects_Characters_Party

#include <vector>

#include "base.hpp"

namespace Game {
    namespace Objects {
        namespace Characters {
            class Party : public Objects::Base {
            public:
                std::vector<std::shared_ptr<Characters::Base>> characters;

                Party();
                ~Party();
                std::shared_ptr<Characters::Base> GetLeader();
                std::shared_ptr<Characters::Base> SetLeader(const int index);

            private:
                std::shared_ptr<Characters::Base> leader;

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
