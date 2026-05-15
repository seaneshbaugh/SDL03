#include "actor.hpp"

namespace Game {
    Actor::Actor() {
    }

    Actor::~Actor() {
    }

    void Actor::Update(const double deltaTime) {
        if (this->moving) {
            switch (this->movementDirection) {
            case 1: // Up
                this->worldY -= this->movementSpeed * deltaTime;

                if (this->worldY <= this->targetTileY * this->currentMap->tileheight) {
                    this->worldY = this->targetTileY * this->currentMap->tileheight;

                    this->moving = false;
                }
                break;
            case 2: // Right
                this->worldX += this->movementSpeed * deltaTime;

                if (this->worldX >= this->targetTileX * this->currentMap->tilewidth) {
                    this->worldX = this->targetTileX * this->currentMap->tilewidth;

                    this->moving = false;
                }
                break;
            case 3: // Down
                this->worldY += this->movementSpeed * deltaTime;

                if (this->worldY >= this->targetTileY * this->currentMap->tileheight) {
                    this->worldY = this->targetTileY * this->currentMap->tileheight;

                    this->moving = false;
                }
                break;
            case 4: // Left
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

            this->spriteName = "walk." + this->SpriteDirection();

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
            this->spriteName = "stand." + this->SpriteDirection();
            this->walkAnimationFrame = 0;
            this->timeSinceLastWalkAnimationFrame = 0.0f;
        }
    }

    void Actor::BeginMovement(const int currentX, const int currentY, const int direction, std::function<void(const int, const int)> endMovementCallback) {
        this->startTileX = currentX;
        this->startTileY = currentY;

        this->targetTileX = this->startTileX;
        this->targetTileY = this->startTileY;

        switch (direction) {
        case 1: // Up
            if (this->targetTileY > 0) {
                this->targetTileY--;
            }

            break;
        case 2: // Right
            if (this->targetTileX < this->currentMap->width - 1) {
                targetTileX++;
            }

            break;
        case 3: // Down
            if (this->targetTileY < this->currentMap->height - 1) {
                this->targetTileY++;
            }

            break;
        case 4: // Left
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
        this->movementDirection = direction;

        this->endMovementCallback = endMovementCallback;
    }

    std::string Actor::SpriteDirection() {
        switch (this->movementDirection) {
        case 1:
            return "up";
        case 2:
            return "right";
        case 3:
            return "down";
        case 4:
            return "left";
        }

        return "down";
    }
 }
