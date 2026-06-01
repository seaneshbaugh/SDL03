#ifndef SDL03_Game_Scene_ActorManager
#define SDL03_Game_Scene_ActorManager

#include "actor.hpp"

namespace Game {
    namespace Scene {
        class ActorManager {
        public:
            std::shared_ptr<Actor> player;
            std::vector<std::shared_ptr<Actor>> actors;
            std::unordered_map<std::string, std::shared_ptr<Actor>> actorLookup;

            ActorManager();
            ~ActorManager();

        private:
        };
    }
}

#endif
