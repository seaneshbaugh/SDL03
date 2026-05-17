#include "actor.hpp"

namespace Game {
    Actor::Actor() {
        this->currentMap = nullptr;
        this->currentTileX = 0;
        this->currentTileY = 0;
        this->animation = Animation::Stand;
        this->direction = Direction::Down;
        this->moving = false;
        this->movementSpeed = 4.0f;
        this->completedStepThisFrame = false;
    }

    Actor::~Actor() {
    }

    int Actor::GetCurrentTileX() const {
        return this->currentTileX;
    }

    int Actor::GetCurrentTileY() const {
        return this->currentTileY;
    }

    float Actor::GetCurrentWorldX() const {
        return this->worldX;
    }

    float Actor::GetCurrentWorldY() const {
        return this->worldY;
    }

    void Actor::SetPosition(const int x, const int y) {
        this->currentTileX = x;
        this->currentTileY = y;

        if (!this->currentMap) {
            this->logger->error() << "Attempted to set position of actor before currentMap was set. Please set currentMap before setting the position.";

            return;
        }

        this->worldX = static_cast<float>(x * this->currentMap->tilewidth);
        this->worldY = static_cast<float>(y * this->currentMap->tileheight);
    }

    Actor::Animation Actor::GetAnimation() const {
        return this->animation;
    }

    void Actor::SetAnimation(const Animation animation) {
        this->animation = animation;

        this->spriteName = this->AnimationToString(this->animation) + "." + this->DirectionToString(this->direction);
    }

    Actor::Direction Actor::GetDirection() const {
        return this->direction;
    }

    void Actor::SetDirection(const Direction direction) {
        this->direction = direction;

        this->spriteName = this->AnimationToString(this->animation) + "." + this->DirectionToString(this->direction);
    }

    bool Actor::IsMoving() const {
        return this->moving;
    }

    void Actor::Update(const double deltaTime) {
        if (this->moving) {
            float movementSpeedX = this->movementSpeed * static_cast<float>(this->currentMap->tilewidth);
            float movementSpeedY = this->movementSpeed * static_cast<float>(this->currentMap->tileheight);
            float targetTileX = static_cast<float>(this->targetTileX * this->currentMap->tilewidth);
            float targetTileY = static_cast<float>(this->targetTileY * this->currentMap->tileheight);

            switch (this->direction) {
            case Direction::Up:
                this->worldY -= movementSpeedY * deltaTime;

                if (this->worldY <= targetTileY) {
                    this->currentTileY = this->targetTileY;
                    this->worldY = targetTileY;

                    this->moving = false;
                }
                break;
            case Direction::Right:
                this->worldX += movementSpeedX * deltaTime;

                if (this->worldX >= targetTileX) {
                    this->currentTileX = this->targetTileX;
                    this->worldX = targetTileX;

                    this->moving = false;
                }
                break;
            case Direction::Down:
                this->worldY += movementSpeedY * deltaTime;

                if (this->worldY >= targetTileY) {
                    this->currentTileY = this->targetTileY;
                    this->worldY = targetTileY;

                    this->moving = false;
                }
                break;
            case Direction::Left:
                this->worldX -= movementSpeedX * deltaTime;

                if (this->worldX <= targetTileX) {
                    this->currentTileX = this->targetTileX;
                    this->worldX = targetTileX;

                    this->moving = false;
                }
                break;
            }

            if (!this->moving) {
                this->completedStepThisFrame = true;
            }

            this->timeSinceLastAnimationFrame += deltaTime;

            // There are 8 frames in the walk animation right now. It's very unlikely that'll ever change, but it'd
            // still be a good idea to not hard code that value here. Maybe add a function to the Character class
            // that returns the number of frames in the walk animation and then use the reciprocal.
            if (this->timeSinceLastAnimationFrame >= 0.125f) {
                this->animationFrame = (this->animationFrame + 1) % 8;
                this->timeSinceLastAnimationFrame = 0.0f;
            }
        } else {
            // I don't like how I'm resetting this on every frame where the player is standing still. But if I set
            // the animation when the player is finished moving in the block above then it drops the last frame
            // of the walk animation and looks really weird.
            this->SetAnimation(Animation::Stand);
            this->animationFrame = 0;
            this->timeSinceLastAnimationFrame = 0.0f;
        }
    }

    void Actor::BeginMovement(const Direction direction, const unsigned int distance) {
        this->startTileX = this->currentTileX;
        this->startTileY = this->currentTileY;

        this->targetTileX = this->startTileX;
        this->targetTileY = this->startTileY;

        switch (direction) {
        case Direction::Up:
            if (this->targetTileY > 0) {
                this->targetTileY--;
            }

            break;
        case Direction::Right:
            if (this->targetTileX < this->currentMap->width - 1) {
                targetTileX++;
            }

            break;
        case Direction::Down:
            if (this->targetTileY < this->currentMap->height - 1) {
                this->targetTileY++;
            }

            break;
        case Direction::Left:
            if (this->targetTileX > 0) {
                this->targetTileX--;
            }

            break;
        }

        // Only start moving if the target tile is different from the starting tile and the target tile is walkable.
        if ((this->targetTileX != this->startTileX || this->targetTileY != this->startTileY) && this->currentMap->GetWalkability(this->targetTileX, this->targetTileY)) {
            this->moving = true;

            this->SetAnimation(Animation::Walk);
        }

        // Always change movement direction so even if we can't move in that direction we still face the correct way.
        this->SetDirection(direction);
    }

    bool Actor::ConsumeCompletedStep() {
        if (this->completedStepThisFrame) {
            this->completedStepThisFrame = false;

            return true;
        }

        return false;
    }

    std::string Actor::AnimationToString(const Animation animation) {
        switch (animation) {
        case Animation::Stand:
            return "stand";
        case Animation::Walk:
            return "walk";
        }

        return "stand";
    }

    std::string Actor::DirectionToString(const Direction direction) {
        switch (this->direction) {
        case Direction::Up:
            return "up";
        case Direction::Right:
            return "right";
        case Direction::Down:
            return "down";
        case Direction::Left:
            return "left";
        }

        return "down";
    }
 }
