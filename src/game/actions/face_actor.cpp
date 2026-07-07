#include "face_actor.hpp"

namespace Game {
    namespace Actions {
        FaceActor::FaceActor(Scenes::Scene* scene, const std::string& actorId, const Scenes::Actor::Direction direction) : scene(scene), actorId(actorId), direction(direction), targetId(""), actor(nullptr), target(nullptr), completed(false) {
        }

        FaceActor::FaceActor(Scenes::Scene* scene, const std::string& actorId, const std::string& targetId) : scene(scene), actorId(actorId), targetId(targetId), direction(Scenes::Actor::Direction::Down), actor(nullptr), target(nullptr), completed(false) {
        }

        FaceActor::~FaceActor() {
        }

        void FaceActor::Start() {
            this->actor = this->scene->GetActor(this->actorId);

            if (!this->actor) {
                this->completed = true;

                return;
            }

            if (!this->targetId.empty()) {
                this->target = this->scene->GetActor(this->targetId);

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
                        this->direction = Scenes::Actor::Direction::Right;
                    } else {
                        this->direction = Scenes::Actor::Direction::Left;
                    }
                } else {
                    if (deltaY > 0) {
                        this->direction = Scenes::Actor::Direction::Down;
                    } else {
                        this->direction = Scenes::Actor::Direction::Up;
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
