#include "pathfind_actor.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                PathfindActor::PathfindActor(States::Map* map, const std::string& actorId, const int targetX, const int targetY) : map(map), actorId(actorId), actor(nullptr), targetX(targetX), targetY(targetY), target(nullptr),started(false) {
                }

                PathfindActor::PathfindActor(States::Map* map, const std::string& actorId, const std::string& targetId) : map(map), actorId(actorId), actor(nullptr), targetX(-1), targetY(-1), targetId(targetId), target(nullptr), started(false) {
                }

                PathfindActor::~PathfindActor() {
                }

                void PathfindActor::Start() {
                    this->actor = this->map->actorManager->GetActor(this->actorId);

                    if (!this->actor) {
                        return;
                    }

                    if (!this->targetId.empty()) {
                        this->target = this->map->actorManager->GetActor(this->targetId);

                        if (!this->target) {
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
                        case Scene::Actor::Direction::Down:
                            std::rotate(possibleTargets.begin(), possibleTargets.begin() + 1, possibleTargets.end());

                            break;
                        case Scene::Actor::Direction::Left:
                            std::rotate(possibleTargets.begin(), possibleTargets.begin() + 2, possibleTargets.end());

                            break;
                        case Scene::Actor::Direction::Right:
                            std::rotate(possibleTargets.begin(), possibleTargets.begin() + 3, possibleTargets.end());

                            break;
                        }

                        // If no possible target is valid then that means the target is surrounded by unwalkable tiles,
                        // in which case we should just try to pathfind to the target's tile and let the pathfinding fail
                        // and not move the actor.
                        for (auto& possibleTarget : possibleTargets) {
                            if (!this->map->IsTileBlocked(possibleTarget.first, possibleTarget.second, this->actor.get())) {
                                this->targetX = possibleTarget.first;
                                this->targetY = possibleTarget.second;

                                break;
                            }
                        }
                    }

                    this->path = this->map->Pathfind(this->actor.get(),this->targetX, this->targetY);

                    for (auto direction : this->path) {
                        this->actor->QueueMovement(direction);
                    }

                    this->started = true;
                }

                void PathfindActor::Update(float deltaTime) {
                }

                bool PathfindActor::IsCompleted() const {
                    return this->started && !this->actor->IsMoving() && !this->actor->HasPendingMovement();
                }
            }
        }
    }
}
