#ifndef __SDL03__WorldService__
#define __SDL03__WorldService__

// #include "GameWorld.h"
#include <memory>

class GameWorld;

namespace Services {
    namespace Interfaces {
        class WorldService {
        public:
            virtual ~WorldService() {}
            virtual std::shared_ptr<GameWorld> GetWorld() = 0;
        };
    }
}

#endif
