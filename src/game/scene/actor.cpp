#include "actor.hpp"

namespace Game {
    namespace Scene {
        Actor::Actor(std::shared_ptr<Graphics::Spritesheet> spritesheet) {
            this->currentMap = nullptr;
            this->currentTileX = 0;
            this->currentTileY = 0;
            this->animation = Animation::Stand;
            this->direction = Direction::Down;
            this->isMoving = false;
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
            if (this->isMoving) {
                return this->movementTargetTileX;
            }

            return this->currentTileX;
        }

        int Actor::GetOccupiedTileY() const {
            if (this->isMoving) {
                return this->movementTargetTileY;
            }

            return this->currentTileY;
        }

        float Actor::GetCurrentWorldX() const {
            return this->currentWorldX;
        }

        float Actor::GetCurrentWorldY() const {
            return this->currentWorldY;
        }

        void Actor::SetPosition(const int x, const int y) {
            this->currentTileX = x;
            this->currentTileY = y;

            if (!this->currentMap) {
                this->logger->error() << "Attempted to set position of actor before currentMap was set. Please set currentMap before setting the position.";

                return;
            }

            // this->currentWorldX = static_cast<float>(x * this->currentMap->tilewidth);
            // this->currentWorldY = static_cast<float>(y * this->currentMap->tileheight);
            this->currentWorldX = static_cast<float>(x * this->currentMap->tilewidth) + (static_cast<float>(this->currentMap->tilewidth) / 2.0f);
            this->currentWorldY = static_cast<float>((y + 1) * this->currentMap->tileheight);
        }

        Actor::Animation Actor::GetAnimation() const {
            return this->animation;
        }

        void Actor::SetAnimation(const Animation animation) {
            this->animation = animation;
        }

        Actor::Direction Actor::GetDirection() const {
            return this->direction;
        }

        void Actor::SetDirection(const Direction direction) {
            this->direction = direction;
        }

        void Actor::SetMovementSpeed(const float movementSpeed) {
            this->movementSpeed = movementSpeed;
        }

        bool Actor::IsMoving() const {
            return this->isMoving;
        }

        void Actor::Update(const double deltaTime) {
            if (this->isMoving) {
                float movementSpeedX = this->movementSpeed * static_cast<float>(this->currentMap->tilewidth);
                float movementSpeedY = this->movementSpeed * static_cast<float>(this->currentMap->tileheight);
                float movementTargetTileX = static_cast<float>(this->movementTargetTileX * this->currentMap->tilewidth) + (static_cast<float>(this->currentMap->tilewidth) / 2.0f);
                float movementTargetTileY = static_cast<float>((this->movementTargetTileY + 1) * this->currentMap->tileheight);

                switch (this->direction) {
                case Direction::Up:
                    this->currentWorldY -= movementSpeedY * deltaTime;

                    if (this->currentWorldY <= movementTargetTileY) {
                        this->currentTileY = this->movementTargetTileY;
                        this->currentWorldY = movementTargetTileY;

                        this->isMoving = false;
                    }
                    break;
                case Direction::Right:
                    this->currentWorldX += movementSpeedX * deltaTime;

                    if (this->currentWorldX >= movementTargetTileX) {
                        this->currentTileX = this->movementTargetTileX;
                        this->currentWorldX = movementTargetTileX;

                        this->isMoving = false;
                    }
                    break;
                case Direction::Down:
                    this->currentWorldY += movementSpeedY * deltaTime;

                    if (this->currentWorldY >= movementTargetTileY) {
                        this->currentTileY = this->movementTargetTileY;
                        this->currentWorldY = movementTargetTileY;

                        this->isMoving = false;
                    }
                    break;
                case Direction::Left:
                    this->currentWorldX -= movementSpeedX * deltaTime;

                    if (this->currentWorldX <= movementTargetTileX) {
                        this->currentTileX = this->movementTargetTileX;
                        this->currentWorldX = movementTargetTileX;

                        this->isMoving = false;
                    }
                    break;
                }

                if (!this->isMoving) {
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

        void Actor::QueueMovement(const Direction direction) {
            this->movementQueue.push(direction);
        }

        std::optional<Actor::Direction> Actor::PeekMovement() const {
           if (this->movementQueue.empty()) {
                return std::nullopt;
           }

           return this->movementQueue.front();
        }

        std::optional<Actor::Direction> Actor::PopMovement() {
            if (this->movementQueue.empty()) {
                return std::nullopt;
            }

            Direction direction = this->movementQueue.front();

            this->movementQueue.pop();

            return direction;
        }

        void Actor::ClearPendingMovement() {
            while (!this->movementQueue.empty()) {
                this->movementQueue.pop();
            }
        }

        void Actor::StartMovement(const Direction direction) {
            if (this->isMoving) {
                return;
            }

            this->movementStartTileX = this->currentTileX;
            this->movementStartTileY = this->currentTileY;

            this->movementTargetTileX = this->movementStartTileX;
            this->movementTargetTileY = this->movementStartTileY;

            switch (direction) {
            case Direction::Up:
                if (this->movementTargetTileY > 0) {
                    this->movementTargetTileY--;
                }

                break;
            case Direction::Right:
                if (this->movementTargetTileX < this->currentMap->width - 1) {
                    movementTargetTileX++;
                }

                break;
            case Direction::Down:
                if (this->movementTargetTileY < this->currentMap->height - 1) {
                    this->movementTargetTileY++;
                }

                break;
            case Direction::Left:
                if (this->movementTargetTileX > 0) {
                    this->movementTargetTileX--;
                }

                break;
            }

            this->isMoving = true;

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
            if (this->isMoving) {
                return this->movementTargetTileX == x && this->movementTargetTileY == y;
            }

            return this->currentTileX == x && this->currentTileY == y;
        }

        void Actor::Render(std::shared_ptr<Camera> camera) {
            this->appearance->Render(this->GetSpriteName(), this->animationFrame, this->currentWorldX, this->currentWorldY, this->currentMap->tileheight, camera);
        }

        std::string Actor::GetSpriteName() const {
            return this->AnimationToString(this->animation) + "." + this->DirectionToString(this->direction);
        }

        std::string Actor::AnimationToString(const Animation animation) const {
            switch (animation) {
            case Animation::Stand:
                return "stand";
            case Animation::Walk:
                return "walk";
            }

            return "stand";
        }

        std::string Actor::DirectionToString(const Direction direction) const {
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
