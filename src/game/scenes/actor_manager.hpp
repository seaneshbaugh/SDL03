#ifndef SDL03_Game_Scene_ActorManager
#define SDL03_Game_Scene_ActorManager

#include "actor.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scenes {
        class Scene;

        class ActorManager {
        public:

            std::shared_ptr<Actor> player;
            std::vector<std::shared_ptr<Actor>> actors;
            std::unordered_map<std::string, std::shared_ptr<Actor>> actorLookup;

            ActorManager(Scene* scene);
            ~ActorManager();
            std::shared_ptr<Scenes::Actor> GetActor(const std::string& id) const;
            std::shared_ptr<Scenes::Actor> AddActor(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const int x, const int y, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);
            void RemoveActor(const std::string& id);
            void PlaceActor(std::shared_ptr<Scenes::Actor> actor, const int x, const int y, const Scenes::Actor::Direction direction) const;
            void UpdateActors(const float deltaTime);
            void RenderActors();

        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            Scene* scene;
        };
    }
}

#endif
