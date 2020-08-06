#ifndef SDL03_Game_Objects_Items_Inventory
#define SDL03_Game_Objects_Items_Inventory

#include <map>
#include <string>
#include <tuple>

#include "consumable.hpp"
#include "equippable.hpp"

namespace Game {
    namespace Objects {
        namespace Items {
            class Inventory {
            public:
                Inventory();
                ~Inventory();
                std::shared_ptr<Items::Consumable> AddItem(std::shared_ptr<Items::Consumable> item);
                std::shared_ptr<Items::Equippable> AddItem(std::shared_ptr<Items::Equippable> item);
                std::tuple<std::shared_ptr<Items::Consumable>, unsigned int> GetConsumable(const std::string& itemId);
                std::tuple<std::shared_ptr<Items::Equippable>, unsigned int> GetEquippable(const std::string& itemId);
                std::map<std::shared_ptr<Items::Consumable>, unsigned int> GetConsumables();
                std::map<std::shared_ptr<Items::Equippable>, unsigned int> GetEquippables();

            private:
                std::map<std::string, std::pair<std::shared_ptr<Items::Base>, unsigned int>> items;
                std::map<std::string, std::pair<std::shared_ptr<Items::Consumable>, unsigned int>> consumables;
                std::map<std::string, std::pair<std::shared_ptr<Items::Equippable>, unsigned int>> equippables;

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
