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
            // straight to the main "world" map. Map loading should probably be paired with a
            // lua script so that way I can load a map and immediately begin a cut scene.
            // I probably want the initial map and starting party to be defined in a config file.
            std::shared_ptr<States::Map> WorldManager::NewGame() {
                Services::Locator::WorldService()->SetWorld(std::make_shared<Objects::World>());

                this->world->SetStartingPlayerParty();

                this->world->LoadMap("world01");

                std::pair<unsigned int, unsigned int> startingPosition = this->world->currentMap->GetDefaultStartPoint();

                this->UpdatePlayerPosition(startingPosition.first, startingPosition.second);

                return std::make_shared<States::Map>();
            }

            void WorldManager::UpdatePlayerPosition(unsigned int x, unsigned int y) {
                this->world->playerCurrentX = x;
                this->world->playerCurrentY = y;
            }
        }
    }
}
