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
            void Update(const float deltaTime);
            void ProcessCompletedSteps();
            void ProcessPendingMovement();
            void Render() const;

        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            States::Map* mapState;
        };
    }
}

#endif
