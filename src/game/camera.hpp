#ifndef SDL03_Game_Camera
#define SDL03_Game_Camera

#include "services/locator.hpp"
#include "actor.hpp"

namespace Game {
    class Camera {
    public:
        Camera(const float x, const float y, const float viewportWidth, const float viewportHeight);
        ~Camera();
        void Follow(std::shared_ptr<Actor> target, int mapWidth, int mapHeight);
        // void Follow(const float targetX, const float targetY, int mapWidth, int mapHeight);

        float x;
        float y;
        float viewportWidth;
        float viewportHeight;
        std::shared_ptr<Actor> target;

    private:
        static const std::string logChannel;

        std::shared_ptr<Log::Logger> logger;
    };
}

#endif
