#include "face_actor.hpp"
#include "../../../states/map.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                FaceActor::FaceActor(States::Map* map, const std::string& actorId, const Actor::Direction direction) : map(map), actorId(actorId), direction(direction), targetId(""), actor(nullptr), target(nullptr), completed(false) {
                }

                FaceActor::FaceActor(States::Map* map, const std::string& actorId, const std::string& targetId) : map(map), actorId(actorId), targetId(targetId), direction(Actor::Direction::Down), actor(nullptr), target(nullptr), completed(false) {
                }

                FaceActor::~FaceActor() {
                }

                void FaceActor::Start() {
                    this->actor = this->map->scene->GetActor(this->actorId);

                    if (!this->actor) {
                        this->completed = true;

                        return;
                    }

                    if (!this->targetId.empty()) {
                        this->target = this->map->scene->GetActor(this->targetId);

                        if (!this->target) {
                            this->completed = true;

                            return;
                        }

                        int deltaX = this->target->GetOccupiedTileX() - this->actor->GetOccupiedTileX();
                        int deltaY = this->target->GetOccupiedTileY() - this->actor->GetOccupiedTileY();

                        if (deltaX == 0 && deltaY == 0) {
                            this->completed = true;

                            return;
                        } else if (std::abs(deltaX) > std::abs(deltaY)) {
                            if (deltaX > 0) {
                                this->direction = Actor::Direction::Right;
                            } else {
                                this->direction = Actor::Direction::Left;
                            }
                        } else {
                            if (deltaY > 0) {
                                this->direction = Actor::Direction::Down;
                            } else {
                                this->direction = Actor::Direction::Up;
                            }
                        }
                    }

                    this->actor->SetDirection(this->direction);

                    this->completed = true;
                }

                void FaceActor::Update(float deltaTime) {
                }

                bool FaceActor::IsCompleted() const {
                    return this->completed;
                }
            }
        }
    }
}
