#include "camera.hpp"

namespace Game {
    const std::string Camera::logChannel = "camera";

    Camera::Camera(const float x, const float y, const float viewportWidth, const float viewportHeight) {
        this->logger = Services::Locator::LoggerService()->GetLogger(Camera::logChannel);

        this->x = x;
        this->y = y;
        this->viewportWidth = viewportWidth;
        this->viewportHeight = viewportHeight;
    }

    Camera::~Camera() {
    }

    void Camera::Follow(std::shared_ptr<Actor> target, int mapWidth, int mapHeight) {
        float desiredX = target->worldX - (this->viewportWidth / 2.0f);
        float desiredY = target->worldY - (this->viewportHeight / 2.0f);

        this->logger->debug() << "desiredX: " << desiredX << ", desiredY: " << desiredY;

        this->x = std::clamp(desiredX, 0.0f, static_cast<float>(mapWidth - this->viewportWidth));
        this->y = std::clamp(desiredY, 0.0f, static_cast<float>(mapHeight - this->viewportHeight));

        this->logger->debug() << "x: " << this->x << ", y: " << this->y;
    }
}
