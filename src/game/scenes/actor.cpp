#include "actor.hpp"
#include "../states/map.hpp"

namespace Game {
    namespace Scenes {
        const std::string Actor::logChannel = "scene.actor";

        std::string Actor::AnimationToString(const Animation animation) {
            switch (animation) {
            case Animation::Die:
                return "die";
            case Animation::Stand:
                return "stand";
            case Animation::Walk:
                return "walk";
            }

            return "stand";
        }

        std::string Actor::DirectionToString(const Direction direction) {
            switch (direction) {
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

        Actor::Animation Actor::StringToAnimation(const std::string& animation) {
            if (animation == "die") {
                return Animation::Die;
            } else if (animation == "idle") {
                return Animation::Idle;
            } else if (animation == "stand") {
                return Animation::Stand;
            } else if (animation == "walk") {
                return Animation::Walk;
            }

            return Animation::Stand;
        }

        Actor::Direction Actor::StringToDirection(const std::string& direction) {
            if (direction == "up") {
                return Direction::Up;
            } else if (direction == "right") {
                return Direction::Right;
            } else if (direction == "down") {
                return Direction::Down;
            } else if (direction == "left") {
                return Direction::Left;
            }

            return Direction::Down;
        }

        Actor::Actor(std::shared_ptr<Graphics::Spritesheet> spritesheet) {
            this->logger = Services::Locator::LoggerService()->GetLogger(Actor::logChannel);
            this->persistent = false;
            this->currentMap = nullptr;
            this->currentTileX = 0;
            this->currentTileY = 0;
            this->animation = Animation::Stand;
            this->isPlayingAnimation = false;
            this->direction = Direction::Down;
            this->isMoving = false;
            this->movementSpeed = 4.0f;
            this->appearance = std::make_shared<ActorAppearance>(spritesheet);
            this->LoadLuaState();
        }

        Actor::~Actor() {
        }

        bool Actor::IsPersistent() const {
            return this->persistent;
        }

        void Actor::SetPersistent(const bool persistent) {
            this->persistent = persistent;
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

        bool Actor::OccupiesTile(const int x, const int y) const {
            if (this->isMoving) {
                return this->movementTargetTileX == x && this->movementTargetTileY == y;
            }

            return this->currentTileX == x && this->currentTileY == y;
        }

        void Actor::SetPosition(const int x, const int y) {
            this->currentTileX = x;
            this->currentTileY = y;

            if (!this->currentMap) {
                this->logger->error() << "Attempted to set position of actor before currentMap was set. Please set currentMap before setting the position.";

                return;
            }

            this->currentWorldX = static_cast<float>(x * this->currentMap->tilewidth) + (static_cast<float>(this->currentMap->tilewidth) / 2.0f);
            this->currentWorldY = static_cast<float>((y + 1) * this->currentMap->tileheight);
        }

        Actor::Animation Actor::GetAnimation() const {
            return this->animation;
        }

        void Actor::SetAnimation(const Animation animation) {
            this->animation = animation;
        }

        int Actor::GetAnimationFrameCount() const {
            return this->appearance->spritesheet->animations.find(this->GetSpriteName())->second.frames.size();
        }

        Actor::Direction Actor::GetDirection() const {
            return this->direction;
        }

        void Actor::SetDirection(const Direction direction) {
            this->direction = direction;
        }

        std::string Actor::GetSpriteName() const {
            return AnimationToString(this->animation) + "." + DirectionToString(this->direction);
        }

        bool Actor::HasPendingMovement() const {
            return !this->movementQueue.empty();
        }

        std::optional<Actor::Direction> Actor::PeekMovement() const {
            if (this->movementQueue.empty()) {
                return std::nullopt;
            }

            return this->movementQueue.front();
        }

        void Actor::QueueMovement(const Direction direction) {
            this->movementQueue.push(direction);
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

        bool Actor::IsMoving() const {
            return this->isMoving;
        }

        void Actor::SetMovementSpeed(const float movementSpeed) {
            this->movementSpeed = movementSpeed;
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

        void Actor::Interact(std::shared_ptr<Actor> target) {
            sol::protected_function onInteract = (*target->luaState.get())["on_interact"];

            if (onInteract.valid()) {
                sol::protected_function_result result = onInteract(this);

                if (!result.valid()) {
                    sol::error err = result;
                    this->logger->error() << "Error in Lua on_interact function: " << err.what();
                }
            }
        }

        void Actor::Update(const float deltaTime) {
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
                    this->animationFrame = (this->animationFrame + 1) % this->GetAnimationFrameCount();
                    this->timeSinceLastAnimationFrame = 0.0f;
                }
            } else {
                if (!this->isPlayingAnimation) {
                    // I don't like how I'm resetting this on every frame where the player is standing still. But if I set
                    // the animation when the player is finished moving in the block above then it drops the last frame
                    // of the walk animation and looks really weird.
                    this->SetAnimation(Animation::Stand);
                    this->animationFrame = 0;
                    this->timeSinceLastAnimationFrame = 0.0f;
                }
            }
        }

        void Actor::GenerateMovementIntent(const float deltaTime) {
        }

        void Actor::Render(std::shared_ptr<Camera> camera) const {
            this->appearance->Render(this->GetSpriteName(), this->animationFrame, this->currentWorldX, this->currentWorldY, camera);
        }

        void Actor::LoadLuaState() {
            this->luaState = std::make_shared<sol::state>();
            this->luaState->open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::math, sol::lib::os);
            this->luaEnvironment = sol::environment(*this->luaState, sol::create, this->luaState->globals());
            this->luaState->set("actor", this);

            Actor::LuaInterface::Bind(this->luaState);
            States::Map::LuaInterface::Bind(this->luaState);
        }

        bool Actor::LoadLuaScript(const std::string& scriptFilePath) {
            try {
                this->logger->debug() << "Loading \"" << scriptFilePath << "\".";

                // this->luaState->script_file(scriptFilePath, this->luaEnvironment);
                this->luaState->script_file(scriptFilePath);

                this->logger->debug() << "Loaded \"" << scriptFilePath << "\".";

                sol::protected_function initialize = (*this->luaState.get())["initialize"];

                if (initialize.valid()) {
                    sol::protected_function_result result = initialize();

                    if (!result.valid()) {
                        sol::error e = result;
                        this->logger->error() << "Error in Lua initialize function: " << e.what();
                    }
                }
            } catch (const sol::error& e) {
                this->logger->error() << "Failed to load Lua script \"" << scriptFilePath << "\": " << e.what();

                return false;
            }

            return true;
        }

        void Actor::SetMapState(States::Map* mapState) {
            this->luaState->set("mapState", mapState);
        }

        void Actor::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            sol::table scene = (*luaState.get())["scene"].get_or_create<sol::table>(sol::new_table());

            scene.new_usertype<Actor>("Actor",
                                     sol::no_constructor,
                                     "name", &Actor::name,
                                     "dialogueId", &Actor::dialogueId,
                                     "getCurrentTileX", &Actor::GetCurrentTileX,
                                     "getCurrentTileY", &Actor::GetCurrentTileY,
                                     "getOccupiedTileX", &Actor::GetOccupiedTileX,
                                     "getOccupiedTileY", &Actor::GetOccupiedTileY,
                                     "isMoving", &Actor::IsMoving,
                                     "clearPendingMovement", &Actor::ClearPendingMovement,
                                     "queueMovement", &Actor::QueueMovement
                                     );
        }
    }
}
