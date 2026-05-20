#include "actor.hpp"

namespace Game {
    namespace Scene {
        Actor::Actor(std::shared_ptr<Graphics::Spritesheet> spritesheet) {
            this->currentMap = nullptr;
            this->currentTileX = 0;
            this->currentTileY = 0;
            this->animation = Animation::Stand;
            this->direction = Direction::Down;
            this->moving = false;
            this->movementSpeed = 4.0f;
            this->appearance = std::make_shared<ActorAppearance>(spritesheet);
        }

        Actor::~Actor() {
        }

        int Actor::GetCurrentTileX() const {
            return this->currentTileX;
        }

        int Actor::GetCurrentTileY() const {
            return this->currentTileY;
        }

        int Actor::GetOccupiedTileX() const {
            if (this->moving) {
                return this->targetWorldX;
            }

            return this->currentTileX;
        }

        int Actor::GetOccupiedTileY() const {
            if (this->moving) {
                return this->targetWorldY;
            }

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

            // this->worldX = static_cast<float>(x * this->currentMap->tilewidth);
            // this->worldY = static_cast<float>(y * this->currentMap->tileheight);
            this->worldX = static_cast<float>(x * this->currentMap->tilewidth) + (static_cast<float>(this->currentMap->tilewidth) / 2.0f);
            this->worldY = static_cast<float>((y + 1) * this->currentMap->tileheight);
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

        void Actor::SetMovementSpeed(const float movementSpeed) {
            this->movementSpeed = movementSpeed;
        }

        bool Actor::IsMoving() const {
            return this->moving;
        }

        void Actor::Update(const double deltaTime) {
            if (this->moving) {
                float movementSpeedX = this->movementSpeed * static_cast<float>(this->currentMap->tilewidth);
                float movementSpeedY = this->movementSpeed * static_cast<float>(this->currentMap->tileheight);
                // float targetWorldX = static_cast<float>(this->targetWorldX * this->currentMap->tilewidth);
                // float targetWorldY = static_cast<float>(this->targetWorldY * this->currentMap->tileheight);
                float targetWorldX = static_cast<float>(this->targetWorldX * this->currentMap->tilewidth) + (static_cast<float>(this->currentMap->tilewidth) / 2.0f);
                float targetWorldY = static_cast<float>((this->targetWorldY + 1) * this->currentMap->tileheight);

                switch (this->direction) {
                case Direction::Up:
                    this->worldY -= movementSpeedY * deltaTime;

                    if (this->worldY <= targetWorldY) {
                        this->currentTileY = this->targetWorldY;
                        this->worldY = targetWorldY;

                        this->moving = false;
                    }
                    break;
                case Direction::Right:
                    this->worldX += movementSpeedX * deltaTime;

                    if (this->worldX >= targetWorldX) {
                        this->currentTileX = this->targetWorldX;
                        this->worldX = targetWorldX;

                        this->moving = false;
                    }
                    break;
                case Direction::Down:
                    this->worldY += movementSpeedY * deltaTime;

                    if (this->worldY >= targetWorldY) {
                        this->currentTileY = this->targetWorldY;
                        this->worldY = targetWorldY;

                        this->moving = false;
                    }
                    break;
                case Direction::Left:
                    this->worldX -= movementSpeedX * deltaTime;

                    if (this->worldX <= targetWorldX) {
                        this->currentTileX = this->targetWorldX;
                        this->worldX = targetWorldX;

                        this->moving = false;
                    }
                    break;
                }

                if (!this->moving) {
                    this->completedSteps.push({this->currentTileX, this->currentTileY});
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

        void Actor::QueueStep(const Direction direction) {
            this->pendingSteps.push(direction);
        }

        std::optional<Actor::Direction> Actor::PeekMove() const {
           if (this->pendingSteps.empty()) {
                return std::nullopt;
           }

           return this->pendingSteps.front();
        }

        std::optional<Actor::Direction> Actor::PopMove() {
            if (this->pendingSteps.empty()) {
                return std::nullopt;
            }

            Direction direction = this->pendingSteps.front();

            this->pendingSteps.pop();

            return direction;
        }

        void Actor::ClearPendingMoves() {
            while (!this->pendingSteps.empty()) {
                this->pendingSteps.pop();
            }
        }

        void Actor::BeginStep(const Direction direction) {
            if (this->moving) {
                return;
            }

            this->startTileX = this->currentTileX;
            this->startTileY = this->currentTileY;

            this->targetWorldX = this->startTileX;
            this->targetWorldY = this->startTileY;

            switch (direction) {
            case Direction::Up:
                if (this->targetWorldY > 0) {
                    this->targetWorldY--;
                }

                break;
            case Direction::Right:
                if (this->targetWorldX < this->currentMap->width - 1) {
                    targetWorldX++;
                }

                break;
            case Direction::Down:
                if (this->targetWorldY < this->currentMap->height - 1) {
                    this->targetWorldY++;
                }

                break;
            case Direction::Left:
                if (this->targetWorldX > 0) {
                    this->targetWorldX--;
                }

                break;
            }

            this->moving = true;

            this->SetAnimation(Animation::Walk);

            // Always change movement direction just in case.
            this->SetDirection(direction);
        }

        bool Actor::HasCompletedSteps() const {
            return !this->completedSteps.empty();
        }

        std::optional<Actor::CompletedStep> Actor::ConsumeCompletedStep() {
            if (this->completedSteps.empty()) {
                return std::nullopt;
            }

            CompletedStep step = this->completedSteps.front();
            this->completedSteps.pop();

            return step;
        }

        bool Actor::OccupiesTile(const int x, const int y) const {
            if (this->moving) {
                return this->targetWorldX == x && this->targetWorldY == y;
            }

            return this->currentTileX == x && this->currentTileY == y;
        }

        void Actor::Render(std::shared_ptr<Camera> camera) {
            this->appearance->Render(this->spriteName, this->animationFrame, this->worldX, this->worldY, this->currentMap->tileheight, camera);
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
}
