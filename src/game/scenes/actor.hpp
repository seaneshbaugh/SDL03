#ifndef SDL03_Game_Scene_Actor
#define SDL03_Game_Scene_Actor

#include <optional>
#include <queue>

#include "../../../lib/lua/src/lua.hpp"
#include "../../../lib/sol/sol.hpp"

#include "../types.hpp"
#include "../interfaces/command_queue.hpp"
#include "../objects/maps/map.hpp"
#include "../scripts/script_selector.hpp"
#include "../services/locator.hpp"
#include "actor_appearance.hpp"
#include "animation_player.hpp"

namespace Game {
    namespace Scenes {
        class Camera;

        class Actor {
        public:
            enum class Animation {
                Die,
                Idle,
                Stand,
                Walk
            };

            struct CompletedStep {
                int tileX;
                int tileY;
            };

            static std::string AnimationToString(const Animation animation);
            static std::string DirectionToString(const Direction direction);
            static Animation StringToAnimation(const std::string& animation);
            static Direction StringToDirection(const std::string& direction);

            std::string id;
            std::string name;
            std::unique_ptr<Scripts::ScriptSelector> scriptSelector;
            std::shared_ptr<Objects::Maps::Map> currentMap;
            std::shared_ptr<ActorAppearance> appearance;
            AnimationPlayer animationPlayer;
            std::shared_ptr<sol::state> luaState;

            Actor(std::shared_ptr<Graphics::Spritesheet> spritesheet);
            ~Actor();

            bool IsPersistent() const;
            void SetPersistent(const bool persistent);

            int GetCurrentTileX() const;
            int GetCurrentTileY() const;
            int GetOccupiedTileX() const;
            int GetOccupiedTileY() const;
            float GetCurrentWorldX() const;
            float GetCurrentWorldY() const;
            bool OccupiesTile(const int x, const int y) const;
            void SetPosition(const int x, const int y);

            Direction GetDirection() const;
            void SetDirection(const Direction direction);
            void SetDirection(Actor* target);

            bool HasPendingMovement() const;
            std::optional<Direction> PeekMovement() const;
            void QueueMovement(const Direction direction);
            std::optional<Direction> PopMovement();
            void ClearPendingMovement();

            bool IsMoving() const;
            void SetMovementSpeed(const float movementSpeed);
            void StartMovement(const Direction direction);

            bool HasCompletedSteps() const;
            std::optional<CompletedStep> ConsumeCompletedStep();

            std::string SelectInteractionScriptId(const World::Conditions::EvaluationContexts::EvaluationContext& context) const;

            void QueueInteraction();
            bool PeekInteraction() const;
            void Interact(std::shared_ptr<Actor> interactor);
            void ConsumeInteraction();

            void Update(const float deltaTime);
            void Render(std::shared_ptr<Camera> camera) const;

            bool LoadLuaScript(const std::string& scriptFilePath);
            void SetCommandQueue(Interfaces::CommandQueue* commandQueue);

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
            std::queue<Direction> movementQueue;
            std::queue<CompletedStep> completedSteps;
            bool interactionQueued;
            sol::environment luaEnvironment;

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
