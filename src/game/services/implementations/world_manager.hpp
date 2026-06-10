#ifndef SDL03_Game_Services_Implementations_WorldManager
#define SDL03_Game_Services_Implementations_WorldManager

#include "../locator.hpp"
#include "../interfaces/world_service.hpp"
#include "../../states/map.hpp"
#include "../../world/state.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Services {
        namespace Implementations {
            class WorldManager : public Interfaces::WorldService {
            public:
                WorldManager();
                WorldManager(std::shared_ptr<World::State> state);
                ~WorldManager();
                std::shared_ptr<World::State> GetState();
                void SetState(std::shared_ptr<World::State> newState);
                std::shared_ptr<States::Map> NewGame();
                void UpdatePlayerPosition(unsigned int x, unsigned int y);

            private:
                static const std::string logChannel;

                std::shared_ptr<Log::Logger> logger;
                std::shared_ptr<World::State> state;
            };
        }
    }
}

#endif
