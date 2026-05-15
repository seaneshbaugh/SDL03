#ifndef SDL03_Game_Player
#define SDL03_Game_Player

#include "services/locator.hpp"
#include "objects/maps/map.hpp"

namespace Game {
class Actor {
public:
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
    int movementDirection;
    std::string spriteName;

    Actor();
    ~Actor();
    void Update(const double deltaTime);
    void BeginMovement(const int currentX, const int currentY, const int direction, std::function<void(const int, const int)> endMovementCallback);

    private:
        static const std::string logChannel;

        std::shared_ptr<Log::Logger> logger;
        std::function<void(const int, const int)> endMovementCallback;

        std::string SpriteDirection();
    };
}

#endif
