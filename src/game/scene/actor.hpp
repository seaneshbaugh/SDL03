#ifndef SDL03_Game_Actor
#define SDL03_Game_Actor

#include <queue>
#include <optional>

#include "../services/locator.hpp"
#include "../objects/maps/map.hpp"
#include "actor_appearance.hpp"

namespace Game {
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

            std::string name;
            std::shared_ptr<Objects::Maps::Map> currentMap;
            unsigned int animationFrame;
            float timeSinceLastAnimationFrame;
            std::string spriteName;
            std::shared_ptr<ActorAppearance> appearance;

            Actor(std::shared_ptr<Graphics::Spritesheet> spritesheet);
            ~Actor();
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
            void Update(const double deltaTime);
            void QueueStep(const Direction direction);
            std::optional<Direction> PeekMove() const;
            std::optional<Direction> PopMove();
            void ClearPendingMoves();
            void BeginStep(const Direction direction);
            bool HasCompletedSteps() const;
            std::optional<CompletedStep> ConsumeCompletedStep();
            bool OccupiesTile(const int x, const int y) const;
            void Render(std::shared_ptr<Camera> camera);

        private:
            static const std::string logChannel;

            std::shared_ptr<Log::Logger> logger;
            int currentTileX;
            int currentTileY;
            float worldX;
            float worldY;
            float movementSpeed;
            bool moving;
            int startTileX;
            int startTileY;
            int targetWorldX;
            int targetWorldY;
            Animation animation;
            Direction direction;
            std::queue<Direction> pendingSteps;
            std::queue<CompletedStep> completedSteps;

            std::string AnimationToString(const Animation animation);
            std::string DirectionToString(const Direction direction);
        };
    }
}

#endif
