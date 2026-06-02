#ifndef SDL03_Game_Scene_Scene
#define SDL03_Game_Scene_Scene

#include "actor_manager.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scenes {
        class Scene {
        public:
            std::shared_ptr<ActorManager> actorManager;

            Scene(States::Map* mapState);
            ~Scene();

        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            States::Map* mapState;
        };
    }
}

#endif
