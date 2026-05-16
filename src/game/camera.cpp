#include "camera.hpp"

namespace Game {
    const std::string Camera::logChannel = "camera";

    Camera::Camera(const float x, const float y, const float viewportWidth, const float viewportHeight) {
        this->logger = Services::Locator::LoggerService()->GetLogger(Camera::logChannel);

        this->x = x;
        this->y = y;
        this->viewportWidth = viewportWidth;
        this->viewportHeight = viewportHeight;
        this->target = nullptr;
        this->panning = false;
        this->panSpeed = 5.0f;
    }

    Camera::~Camera() {
    }

    void Camera::Follow(std::shared_ptr<Actor> target) {
        this->target = target;
    }

    // TODO: Store the current map so we always have a reference to the width and height?
    void Camera::Update(const double deltaTime, int mapWidth, int mapHeight) {
        if (!target) {
            return;
        }

        float desiredX = std::clamp(target->worldX - (this->viewportWidth / 2.0f), 0.0f, static_cast<float>(mapWidth) - this->viewportWidth);
        float desiredY = std::clamp(target->worldY - (this->viewportHeight / 2.0f), 0.0f, static_cast<float>(mapHeight) - this->viewportHeight);

        this->logger->debug() << "desiredX: " << desiredX << ", desiredY: " << desiredY;

        if (this->panning) {
            this->x += (desiredX - this->x) * this->panSpeed * deltaTime;
            this->y += (desiredY - this->y) * this->panSpeed * deltaTime;
        } else {
            this->x = desiredX;
            this->y = desiredY;
        }
        
        this->logger->debug() << "x: " << this->x << ", y: " << this->y;
    }
}
