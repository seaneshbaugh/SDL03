#include "actor.hpp"

namespace Game {
    Actor::Actor() {
        this->animation = Animation::Stand;
        this->direction = Direction::Down;
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
                this->endMovementCallback(this->targetTileX, this->targetTileY);
            }

            this->SetSprite(Animation::Walk, this->direction);

            this->spriteName = "walk." + this->DirectionToString(this->direction);

            this->timeSinceLastWalkAnimationFrame += deltaTime;

            // There are 8 frames in the walk animation right now. It's very unlikely that'll ever change, but it'd
            // still be a good idea to not hard code that value here. Maybe add a function to the Character class
            // that returns the number of frames in the walk animation and then use the reciprocal.
            if (this->timeSinceLastWalkAnimationFrame >= 0.125f) {
                this->walkAnimationFrame = (this->walkAnimationFrame + 1) % 8;
                this->timeSinceLastWalkAnimationFrame = 0.0f;
            }
        } else {
            // I don't like how I'm resetting this on every frame where the player is standing still. It feels wasteful.
            // But I'm already running at like 2000 FPS on my computer so I don't think it's a big deal. If it becomes
            // a problem then I can always add a separate variable to track the player's current sprite and only update
            // it when the direction changes or something like that.
            this->spriteName = "stand." + this->DirectionToString(this->direction);
            this->walkAnimationFrame = 0;
            this->timeSinceLastWalkAnimationFrame = 0.0f;
        }
    }

    void Actor::BeginMovement(const int currentX, const int currentY, const Direction direction, std::function<void(const int, const int)> endMovementCallback) {
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
        }

        // Always change movement direction so even if we can't move in that direction we still face the correct way.
        this->SetDirection(direction);

        this->endMovementCallback = endMovementCallback;
    }

    void Actor::SetSprite(const Animation animation, const Direction direction) {
        this->animation = animation;

        this->direction = direction;

        this->spriteName = this->AnimationToString(this->animation) + "." + this->DirectionToString(this->direction);
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
