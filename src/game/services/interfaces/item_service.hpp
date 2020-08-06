#ifndef SDL03_Game_Services_Interfaces_ItemService
#define SDL03_Game_Services_Interfaces_ItemService

#include "../../objects/items/base.hpp"

namespace Game {
    namespace Services {
        namespace Interfaces {
            class ItemService {
            public:
                virtual ~ItemService() {}
                virtual std::shared_ptr<Objects::Items::Base> GetItem(const std::string& itemId) = 0;
            };
        }
    }
}

#endif
