#include "WorldManager.h"

namespace Services {
    namespace Implementations {
        WorldManager::WorldManager(std::shared_ptr<GameWorld> world) {
            this->world = world;
        }

        WorldManager::~WorldManager() {
        }

        std::shared_ptr<GameWorld> WorldManager::GetWorld() {
            return this->world;
        }
    }
}
