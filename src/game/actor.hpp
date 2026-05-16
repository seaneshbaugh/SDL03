#ifndef SDL03_Game_Actor
#define SDL03_Game_Actor

#include "services/locator.hpp"
#include "objects/maps/map.hpp"

namespace Game {
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
        Walk
    };

    std::shared_ptr<Objects::Maps::Map> currentMap;
    float worldX;
    float worldY;
    unsigned int animationFrame;
    float timeSinceLastAnimationFrame;
    float movementSpeed;
    bool moving;
    int startTileX;
    int startTileY;
    int targetTileX;
    int targetTileY;
    std::string spriteName;

    Actor();
    ~Actor();
    Animation GetAnimation();
    void SetAnimation(const Animation animation);
    Direction GetDirection();
    void SetDirection(const Direction direction);
    void Update(const double deltaTime);
    void BeginMovement(const int currentX, const int currentY, const Direction direction);
    bool ConsumeCompletedStep();

    private:
        static const std::string logChannel;

        std::shared_ptr<Log::Logger> logger;
        Animation animation;
        Direction direction;
        bool completedStepThisFrame;
        std::function<void(const int, const int)> endMovementCallback;

        std::string AnimationToString(const Animation animation);
        std::string DirectionToString(const Direction direction);
    };
}

#endif
