#ifndef SDL03_Game_Objects_Characters_Party
#define SDL03_Game_Objects_Characters_Party

#include <vector>

#include "base.hpp"
#include "../items/inventory.hpp"
#include "../items/consumable.hpp"

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
                std::shared_ptr<Items::Inventory> GetInventory();

            private:
                std::shared_ptr<Characters::Base> leader;
                std::shared_ptr<Items::Inventory> inventory;

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
