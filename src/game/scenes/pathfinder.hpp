#ifndef SDL03_Game_Scenes_Pathfinder
#define SDL03_Game_Scenes_Pathfinder

#include "../services/locator.hpp"
#include "actor.hpp"

namespace Game {
    namespace Scenes {
        class Scene;

        class Pathfinder {
        public:
            Pathfinder(Scene* scene);
            ~Pathfinder();
            std::vector<Actor::Direction> Pathfind(const Actor* actor, const int targetX, const int targetY);

        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            Scene* scene;
        };
    }
}

#endif
