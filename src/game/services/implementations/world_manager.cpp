#include "../../states/map.hpp"
#include "world_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string WorldManager::logChannel = "world";

            WorldManager::WorldManager() {
                this->logger = Locator::LoggerService()->GetLogger(WorldManager::logChannel);
            }

            WorldManager::WorldManager(std::shared_ptr<Objects::World> world) : WorldManager() {
                this->world = world;
            }

            WorldManager::~WorldManager() {
            }

            void WorldManager::SetWorld(std::shared_ptr<Objects::World> newWorld) {
                this->world = newWorld;

                Services::Locator::TimeService()->StartClock(0);
            }

            std::shared_ptr<Objects::World> WorldManager::GetWorld() {
                return this->world;
            }

            // Eventually this will load an initial cutscene. For now it'll just go
            // straight to the main "world" map.
            // TODO: Consider making an actual NewGame state that just loads the world
            // on initialization and then immediately returns the Map state on Update.
            std::shared_ptr<States::Map> WorldManager::NewGame() {
                Services::Locator::WorldService()->SetWorld(std::make_shared<Objects::World>());

                return std::make_shared<States::Map>();
            }
        }
    }
}
