#ifndef SDL03_Game_Services_Implementations_ItemManager
#define SDL03_Game_Services_Implementations_ItemManager

#include <map>

#include "../../../../lib/nlohmann/json.hpp"

#include "../locator.hpp"
#include "../interfaces/item_service.hpp"
#include "../../objects/items/consumable.hpp"
#include "../../objects/items/equippable.hpp"
#include "../../../helpers/file_system.hpp"

using json = nlohmann::json;

namespace Game {
    namespace Services {
        namespace Implementations {
            class ItemManager : public Interfaces::ItemService {
            public:
                ItemManager();
                ~ItemManager();
                std::shared_ptr<Objects::Items::Base> GetItem(const std::string& itemId);

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                std::map<std::string, std::string> registry;
                std::map<std::string, std::shared_ptr<Objects::Items::Base>> items;
            };
        }
    }
}

#endif
