#include "item_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string ItemManager::logChannel = "items";

            ItemManager::ItemManager() {
                this->logger = Locator::LoggerService()->GetLogger(ItemManager::logChannel);

                // TODO: Move this to its own class. This is just a test.
                std::string registryFileContents;

                if (!Helpers::FileSystem::ReadFile("resources/items/registry.json", registryFileContents)) {
                    throw std::runtime_error("Error loading item registry file.");
                }

                json registryNode = json::parse(registryFileContents);

                for (auto it = registryNode.begin(); it != registryNode.end(); ++it) {
                    this->registry[it.key()] = it->get<std::string>();
                }
            }

            ItemManager::~ItemManager() {
            }

            std::shared_ptr<Objects::Items::Base> ItemManager::GetItem(const std::string& itemId) {
                if (this->items.find(itemId) != this->items.end()) {
                    return this->items[itemId];
                }

                if (this->registry.find(itemId) == this->registry.end()) {
                    // TODO: Figure out better way to handle this...
                    return nullptr;
                }

                std::string itemFileContents;

                if (!Helpers::FileSystem::ReadFile(this->registry[itemId], itemFileContents)) {
                    throw std::runtime_error("Error loading item file.");
                }

                json itemNode = json::parse(itemFileContents);

                Objects::Items::ItemType itemType = Objects::Items::ItemTypeNameToEnum(itemNode["type"]);

                // TODO: Create separate item parsers which load all the attributes.
                switch (itemType) {
                case Objects::Items::ItemType::consumable: {
                    std::shared_ptr<Objects::Items::Consumable> consumable = std::make_shared<Objects::Items::Consumable>();

                    consumable->SetId(itemNode["id"]);
                    consumable->SetName(itemNode["name"]);

                    return consumable;
                    }
                case Objects::Items::ItemType::equippable: {
                    std::shared_ptr<Objects::Items::Equippable> equippable = std::make_shared<Objects::Items::Equippable>();

                    equippable->SetId(itemNode["id"]);
                    equippable->SetName(itemNode["name"]);

                    return equippable;
                    }
                default:
                    return nullptr;
                }
            }
        }
    }
}
