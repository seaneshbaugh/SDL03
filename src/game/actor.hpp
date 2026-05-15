#ifndef SDL03_Game_Player
#define SDL03_Game_Player

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
    float screenX;
    float screenY;
    unsigned int walkAnimationFrame;
    float timeSinceLastWalkAnimationFrame;
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
    void BeginMovement(const int currentX, const int currentY, const Direction direction, std::function<void(const int, const int)> endMovementCallback);

    private:
        static const std::string logChannel;

        std::shared_ptr<Log::Logger> logger;
        Animation animation;
        Direction direction;
        std::function<void(const int, const int)> endMovementCallback;

        void SetSprite(const Animation animation, const Direction direction);
        std::string AnimationToString(const Animation animation);
        std::string DirectionToString(const Direction direction);
    };
}

#endif
