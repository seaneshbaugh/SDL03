#ifndef SDL03_Game_Objects_Items_Base
#define SDL03_Game_Objects_Items_Base

#include "../base.hpp"

namespace Game {
    namespace Objects {
        namespace Items {
            enum class ItemType : int {
                consumable      = 1,
                equippable      = 2,
                key             = 3
            };

            enum class ItemSubType : int {
                restorative     = 1,
                armor           = 2,
                weapon          = 3
            };

            namespace {
                static const std::map<std::string, ItemType> itemTypeMap;
                static const std::map<std::string, ItemSubType> itemSubTypeMap;
            }

            ItemType ItemTypeNameToEnum(const std::string& typeName);
            ItemSubType ItemSubTypeNameToEnum(const std::string& subTypeName);

            class Base : public Objects::Base {
            public:
                virtual ~Base();
                std::string GetId();
                std::string SetId(const std::string& id);
                std::string GetName();
                std::string SetName(const std::string& name);

            private:
                std::string id;
                std::string name;
            };
        }
    }
}

#endif
