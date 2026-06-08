#ifndef SDL03_Game_Scene_Scene
#define SDL03_Game_Scene_Scene

#include "actor_manager.hpp"
#include "pathfinder.hpp"
#include "controllers/cutscene_controller.hpp"
#include "controllers/player_controller.hpp"
#include "controllers/scripted_controller.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scenes {
        class Scene {
        public:
            std::shared_ptr<ActorManager> actorManager;
            std::shared_ptr<Camera> camera;
            States::Map* mapState;

            Scene(States::Map* mapState, Objects::Maps::Map* currentMap);
            ~Scene();

            void Update(const float deltaTime);
            void EnqueueMovement(const float deltaTime);
            void ProcessCompletedSteps();
            void ProcessPendingMovement();
            void ProcessInteractions() const;
            void Render() const;

            std::shared_ptr<Scenes::Actor> GetActor(const std::string& id);
            std::optional<std::shared_ptr<Scenes::Actor>> GetActorAtTile(const int x, const int y) const;
            template <typename TController> std::shared_ptr<Scenes::Actor> AddActor(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const int x, const int y, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);
            void RemoveActor(const std::string& id);
            void PlaceActor(std::shared_ptr<Scenes::Actor> actor, const int x, const int y, const Scenes::Actor::Direction direction) const;
            void SetActorController(const std::string& id, std::unique_ptr<Controllers::ActorController> controller);

            void PathfindActor(const std::string& actorId, const int targetX, const int targetY);
            void PathfindActor(Actor* actor, const int targetX, const int targetY);
            bool IsTileBlocked(const int x, const int y, const Scenes::Actor* ignore) const;

            Objects::Maps::Map* GetCurrentMap() const;
            void SetCurrentMap(Objects::Maps::Map* map);
        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            Objects::Maps::Map* currentMap;
            std::unordered_map<std::string, std::unique_ptr<Controllers::ActorController>> actorControllers;
            std::unique_ptr<Pathfinder> pathfinder;

            bool CanMove(Scenes::Actor* actor, const Scenes::Actor::Direction direction) const;
        };
    }
}

#endif
