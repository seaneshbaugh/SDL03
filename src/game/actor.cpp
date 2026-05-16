#include "actor.hpp"

namespace Game {
    Actor::Actor() {
        this->animation = Animation::Stand;
        this->direction = Direction::Down;
        this->completedStepThisFrame = false;
    }

    Actor::~Actor() {
    }

    Actor::Animation Actor::GetAnimation() {
        return this->animation;
    }

    void Actor::SetAnimation(const Animation animation) {
        this->animation = animation;

        this->spriteName = this->AnimationToString(this->animation) + "." + this->DirectionToString(this->direction);
    }

    Actor::Direction Actor::GetDirection() {
        return this->direction;
    }

    void Actor::SetDirection(const Direction direction) {
        this->direction = direction;

        this->spriteName = this->AnimationToString(this->animation) + "." + this->DirectionToString(this->direction);
    }

    void Actor::Update(const double deltaTime) {
        if (this->moving) {
            switch (this->direction) {
            case Direction::Up:
                this->worldY -= this->movementSpeed * deltaTime;

                if (this->worldY <= this->targetTileY * this->currentMap->tileheight) {
                    this->worldY = this->targetTileY * this->currentMap->tileheight;

                    this->moving = false;
                }
                break;
            case Direction::Right:
                this->worldX += this->movementSpeed * deltaTime;

                if (this->worldX >= this->targetTileX * this->currentMap->tilewidth) {
                    this->worldX = this->targetTileX * this->currentMap->tilewidth;

                    this->moving = false;
                }
                break;
            case Direction::Down:
                this->worldY += this->movementSpeed * deltaTime;

                if (this->worldY >= this->targetTileY * this->currentMap->tileheight) {
                    this->worldY = this->targetTileY * this->currentMap->tileheight;

                    this->moving = false;
                }
                break;
            case Direction::Left:
                this->worldX -= this->movementSpeed * deltaTime;

                if (this->worldX <= this->targetTileX * this->currentMap->tilewidth) {
                    this->worldX = this->targetTileX * this->currentMap->tilewidth;

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

    void Actor::BeginMovement(const int currentX, const int currentY, const Direction direction) {
        this->startTileX = currentX;
        this->startTileY = currentY;

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
