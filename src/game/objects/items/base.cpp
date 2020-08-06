#include "base.hpp"

namespace Game {
    namespace Objects {
        namespace Items {
            const std::map<std::string, ItemType> itemTypeMap = {
                {"consumable", ItemType::consumable},
                {"equippable", ItemType::equippable},
                {"key", ItemType::key}
            };

            const std::map<std::string, ItemSubType> itemSubTypeMap = {
                {"resorative", ItemSubType::restorative},
                {"weapon", ItemSubType::weapon},
                {"armor", ItemSubType::armor}
            };

            ItemType ItemTypeNameToEnum(const std::string& itemTypeName) {
                return Items::itemTypeMap.at(itemTypeName);
            }

            ItemSubType ItemSubTypeNameToEnum(const std::string& itemSubTypeName) {
                return Items::itemSubTypeMap.at(itemSubTypeName);
            }

            Base::~Base() {
            }

            std::string Base::GetId() {
                return this->id;
            }

            std::string Base::SetId(const std::string& id) {
                this->id = id;

                return this->id;
            }

            std::string Base::GetName() {
                return this->name;
            }

            std::string Base::SetName(const std::string& name) {
                this->name = name;

                return this->name;
            }
        }
    }
}
