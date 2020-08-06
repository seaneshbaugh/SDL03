#include "inventory.hpp"

namespace Game {
    namespace Objects {
        namespace Items {
            Inventory::Inventory() {
            }

            Inventory::~Inventory() {
            }

            std::shared_ptr<Items::Consumable> Inventory::AddItem(std::shared_ptr<Items::Consumable> item) {
                if (this->consumables.find(item->GetId()) == this->consumables.end()) {
                    this->consumables[item->GetId()] = std::make_tuple(item, 1);
                } else {
                    std::get<1>(this->consumables[item->GetId()])++;
                }

                return item;
            }

            std::shared_ptr<Items::Equippable> Inventory::AddItem(std::shared_ptr<Items::Equippable> item) {
                if (this->equippables.find(item->GetId()) == this->equippables.end()) {
                    this->equippables[item->GetId()] = std::make_tuple(item, 1);
                } else {
                    std::get<1>(this->equippables[item->GetId()])++;
                }

                return item;
            }

            std::tuple<std::shared_ptr<Items::Consumable>, unsigned int> Inventory::GetConsumable(const std::string& itemId) {
                if (this->consumables.find(itemId) != this->consumables.end()) {
                    return this->consumables[itemId];
                }

                return std::make_tuple(nullptr, 0);
            }

            std::map<std::shared_ptr<Items::Consumable>, unsigned int> Inventory::GetConsumables() {
                std::map<std::shared_ptr<Items::Consumable>, unsigned int> result;

                for (auto it = this->consumables.begin(); it != this->consumables.end(); ++it) {
                    result[it->second.first] = it->second.second;
                }

                return result;
            }

            std::map<std::shared_ptr<Items::Equippable>, unsigned int> Inventory::GetEquippables() {
                std::map<std::shared_ptr<Items::Equippable>, unsigned int> result;

                for (auto it = this->equippables.begin(); it != this->equippables.end(); ++it) {
                    result[it->second.first] = it->second.second;
                }

                return result;
            }

            void Inventory::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                sol::table items = objects["items"].get_or_create<sol::table>(sol::new_table());

                items.new_usertype<Inventory>("Inventory",
                                              sol::constructors<Inventory()>(),
                                              "addItem", static_cast<std::shared_ptr<Items::Consumable> (Inventory::*)(std::shared_ptr<Items::Consumable> item)>(&Inventory::AddItem),
                                              "addItem", static_cast<std::shared_ptr<Items::Equippable> (Inventory::*)(std::shared_ptr<Items::Equippable> item)>(&Inventory::AddItem),
                                              "getConsumables", &Inventory::GetConsumables,
                                              "getEquippables", &Inventory::GetEquippables
                                              );
            }
        }
    }
}
