#ifndef __SDL03__WorldManager__
#define __SDL03__WorldManager__

#include <memory>

#include "../interfaces/WorldService.h"

namespace Services {
    namespace Implementations {
        class WorldManager : public Interfaces::WorldService {
        public:
            WorldManager(std::shared_ptr<GameWorld> world);
            ~WorldManager();
            std::shared_ptr<GameWorld> GetWorld();
        private:
            std::shared_ptr<GameWorld> world;
        };
    }
}

#endif
