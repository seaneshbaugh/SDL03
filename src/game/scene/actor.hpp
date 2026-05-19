#ifndef SDL03_Game_Actor
#define SDL03_Game_Actor

#include <queue>
#include <optional>

#include "../services/locator.hpp"
#include "../objects/maps/map.hpp"
#include "actor_appearance.hpp"

namespace Game {
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

        struct CompletedStep {
            int tileX;
            int tileY;
        };

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
        float GetCurrentWorldX() const;
        float GetCurrentWorldY() const;
        Animation GetAnimation() const;
        void SetAnimation(const Animation animation);
        Direction GetDirection() const;
        void SetDirection(const Direction direction);
        bool IsMoving() const;
        void Update(const double deltaTime);
        void BeginMovement(const Direction direction, const unsigned int distance);
        bool HasCompletedSteps() const;
        std::optional<CompletedStep> ConsumeCompletedStep();
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
        std::queue<CompletedStep> completedSteps;
        std::function<void(const int, const int)> endMovementCallback;

        std::string AnimationToString(const Animation animation);
        std::string DirectionToString(const Direction direction);
    };
}

#endif
