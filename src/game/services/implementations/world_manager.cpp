#include "world_manager.hpp"

namespace Game {
    namespace Services {
        namespace Implementations {
            const std::string WorldManager::logChannel = "world";

            WorldManager::WorldManager() {
                this->logger = Locator::LoggerService()->GetLogger(WorldManager::logChannel);
            }

            WorldManager::WorldManager(std::shared_ptr<World::State> state) : WorldManager() {
                this->state = state;
            }

            WorldManager::~WorldManager() {
            }

            void WorldManager::SetState(std::shared_ptr<World::State> newState) {
                this->state = newState;

                Services::Locator::TimeService()->StartClock(0);
            }

            std::shared_ptr<World::State> WorldManager::GetState() {
                return this->state;
            }

            // Eventually this will load an initial cutscene. For now it'll just go
            // straight to the main "world" map. Map loading should probably be paired with a
            // lua script so that way I can load a map and immediately begin a cut scene.
            // I probably want the initial map and starting party to be defined in a config file.
            std::shared_ptr<States::Map> WorldManager::NewGame() {
                Services::Locator::WorldService()->SetState(std::make_shared<World::State>());

                this->state->SetStartingPlayerParty();

                this->state->LoadMap("world01");

                std::pair<unsigned int, unsigned int> startingPosition = this->state->currentMap->GetDefaultStartPoint();

                this->UpdatePlayerPosition(startingPosition.first, startingPosition.second);

                return std::make_shared<States::Map>();
            }

            void WorldManager::UpdatePlayerPosition(unsigned int x, unsigned int y) {
                this->state->playerCurrentX = x;
                this->state->playerCurrentY = y;
            }
        }
    }
}
