#include "pathfind_actor.hpp"

namespace Game {
    namespace Actions {
        PathfindActor::PathfindActor(Scenes::Scene* scene, const std::string& actorId, const int targetX, const int targetY) : scene(scene), actorId(actorId), actor(nullptr), targetX(targetX), targetY(targetY), target(nullptr),started(false), failed(false) {
        }

        PathfindActor::PathfindActor(Scenes::Scene* scene, const std::string& actorId, const std::string& targetId) : scene(scene), actorId(actorId), actor(nullptr), targetX(-1), targetY(-1), targetId(targetId), target(nullptr), started(false), failed(false) {
        }

        PathfindActor::~PathfindActor() {
        }

        void PathfindActor::Start() {
            this->actor = this->scene->GetActor(this->actorId);

            if (!this->actor) {
                this->failed = true;

                return;
            }

            if (!this->targetId.empty()) {
                this->target = this->scene->GetActor(this->targetId);

                if (!this->target) {
                    this->failed = true;

                    return;
                }

                this->targetX = this->target->GetOccupiedTileX();
                this->targetY = this->target->GetOccupiedTileY();

                std::vector<std::pair<int, int>> possibleTargets = {
                    {this->targetX, this->targetY - 1},
                    {this->targetX, this->targetY + 1},
                    {this->targetX - 1, this->targetY},
                    {this->targetX + 1, this->targetY}};

                switch (this->target->GetDirection()) {
                case Scenes::Actor::Direction::Down:
                    std::rotate(possibleTargets.begin(), possibleTargets.begin() + 1, possibleTargets.end());

                    break;
                case Scenes::Actor::Direction::Left:
                    std::rotate(possibleTargets.begin(), possibleTargets.begin() + 2, possibleTargets.end());

                    break;
                case Scenes::Actor::Direction::Right:
                    std::rotate(possibleTargets.begin(), possibleTargets.begin() + 3, possibleTargets.end());

                    break;
                }

                // If no possible target is valid then that means the target is surrounded by unwalkable tiles,
                // in which case we should just try to pathfind to the target's tile and let the pathfinding fail
                // and not move the actor.
                for (auto& possibleTarget : possibleTargets) {
                    if (!this->scene->IsTileBlocked(possibleTarget.first, possibleTarget.second, this->actor.get())) {
                        this->targetX = possibleTarget.first;
                        this->targetY = possibleTarget.second;

                        break;
                    }
                }
            }

            this->scene->PathfindActor(this->actor.get(), this->targetX, this->targetY);

            this->started = true;
        }

        void PathfindActor::Update(float deltaTime) {
        }

        bool PathfindActor::IsCompleted() const {
            return this->failed || (this->started && !this->actor->IsMoving() && !this->actor->HasPendingMovement());
        }
    }
}
