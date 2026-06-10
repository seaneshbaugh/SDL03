#ifndef SDL03_Game_Services_Interfaces_WorldService
#define SDL03_Game_Services_Interfaces_WorldService

#include <memory>

namespace Game {
    namespace States {
        class Map;
    }

    namespace World {
        class State;
    }

    namespace Services {
        namespace Interfaces {
            class WorldService {
            public:
                virtual ~WorldService() {}
                virtual std::shared_ptr<World::State> GetState() = 0;
                virtual void SetState(std::shared_ptr<World::State> newState) = 0;
                virtual std::shared_ptr<States::Map> NewGame() = 0;
                virtual void UpdatePlayerPosition(unsigned int x, unsigned int y) = 0;
            };
        }
    }
}

#endif
