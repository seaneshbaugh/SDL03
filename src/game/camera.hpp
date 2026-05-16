#ifndef SDL03_Game_Camera
#define SDL03_Game_Camera

#include "services/locator.hpp"
#include "actor.hpp"

namespace Game {
    class Camera {
    public:
        Camera(const float x, const float y, const float viewportWidth, const float viewportHeight);
        ~Camera();
        void Follow(std::shared_ptr<Actor> target);
        void Update(const double deltaTime, int mapWidth, int mapHeight);

        float x;
        float y;
        float viewportWidth;
        float viewportHeight;
        bool panning;
        float panSpeed;

    private:
        static const std::string logChannel;

        std::shared_ptr<Log::Logger> logger;
        std::shared_ptr<Actor> target;
    };
}

#endif
