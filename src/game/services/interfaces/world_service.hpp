#ifndef SDL03_Game_Services_Interfaces_WorldService
#define SDL03_Game_Services_Interfaces_WorldService

#include <memory>

class World;

namespace Game {
    namespace States {
        class Map;
    }

    namespace Objects {
        class World;
    }

    namespace Services {
        namespace Interfaces {
            class WorldService {
            public:
                virtual ~WorldService() {}
                virtual std::shared_ptr<Objects::World> GetWorld() = 0;
                virtual void SetWorld(std::shared_ptr<Objects::World> newWorld) = 0;
                virtual std::shared_ptr<States::Map> NewGame() = 0;
            };
        }
    }
}

#endif
