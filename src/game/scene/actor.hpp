#ifndef SDL03_Game_Scene_Actor
#define SDL03_Game_Scene_Actor

#include <queue>
#include <optional>

#include "../../../lib/lua/src/lua.hpp"
#include "../../../lib/sol/sol.hpp"

#include "../services/locator.hpp"
#include "../objects/maps/map.hpp"
#include "actor_appearance.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scene {
        class Camera;

        class Actor {
        public:
            enum class Direction {
                Up,
                Right,
                Down,
                Left
            };

            enum class Animation {
                Stand,
                Walk,
                Idle
            };

            struct AnimationState {
                Animation animation;
                Direction direction;
            };

            struct CompletedStep {
                int tileX;
                int tileY;
            };

            std::string id;
            std::string name;
            std::shared_ptr<Objects::Maps::Map> currentMap;
            unsigned int animationFrame;
            float timeSinceLastAnimationFrame;
            std::shared_ptr<ActorAppearance> appearance;
            // TODO: Make this private?
            std::shared_ptr<sol::state> luaState;
            std::string dialogueId;

            Actor(std::shared_ptr<Graphics::Spritesheet> spritesheet);
            ~Actor();
            bool IsPersistent() const;
            void SetPersistent(const bool persistent);
            void SetPosition(const int x, const int y);
            int GetCurrentTileX() const;
            int GetCurrentTileY() const;
            int GetOccupiedTileX() const;
            int GetOccupiedTileY() const;
            float GetCurrentWorldX() const;
            float GetCurrentWorldY() const;
            Animation GetAnimation() const;
            void SetAnimation(const Animation animation);
            Direction GetDirection() const;
            void SetDirection(const Direction direction);
            void SetMovementSpeed(const float movementSpeed);
            bool IsMoving() const;
            void Update(const float deltaTime);
            void QueueMovement(const Direction direction);
            bool HasPendingMovement() const;
            std::optional<Direction> PeekMovement() const;
            std::optional<Direction> PopMovement();
            void ClearPendingMovement();
            void StartMovement(const Direction direction);
            bool HasCompletedSteps() const;
            std::optional<CompletedStep> ConsumeCompletedStep();
            bool OccupiesTile(const int x, const int y) const;
            void Interact(std::shared_ptr<Actor> interactor);
            void Render(std::shared_ptr<Camera> camera);
            bool LoadLuaScript(const std::string& scriptFilePath);
            void SetMapState(States::Map* mapState);

        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            bool persistent;
            int currentTileX;
            int currentTileY;
            float currentWorldX;
            float currentWorldY;
            float movementSpeed;
            bool isMoving;
            int movementStartTileX;
            int movementStartTileY;
            int movementTargetTileX;
            int movementTargetTileY;
            Animation animation;
            Direction direction;
            std::queue<Direction> movementQueue;
            std::queue<CompletedStep> completedSteps;
            sol::environment luaEnvironment;

            std::string GetSpriteName() const;
            std::string AnimationToString(const Animation animation) const;
            std::string DirectionToString(const Direction direction) const;
            void LoadLuaState();

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<sol::state> luaState);
            };
        };
    }
}

#endif
