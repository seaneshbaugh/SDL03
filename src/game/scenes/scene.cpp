#include "scene.hpp"
#include "../states/map.hpp"

namespace Game {
    namespace Scenes {
        const std::string Scene::logChannel = "scene";

        Scene::Scene(States::Map* mapState) : mapState(mapState) {
            this->logger = Services::Locator::LoggerService()->GetLogger(Scene::logChannel);
            this->actorManager = std::make_shared<ActorManager>(this->mapState);
        }

        Scene::~Scene() {
        }

        void Scene::Update(const float deltaTime) {
            this->actorManager->UpdateActors(deltaTime);
        }

        void Scene::Render() const {
            this->actorManager->RenderActors();
        }

        void Scene::ProcessCompletedSteps() {
            for (auto& actor : this->actorManager->actors) {
                while (auto step = this->actorManager->player->ConsumeCompletedStep()) {
                    // TODO: Remove this check to see if the actor is the player. Eventually it shouldn't needed.
                    if (actor == this->actorManager->player) {
                        Services::Locator::WorldService()->UpdatePlayerPosition(step->tileX, step->tileY);

                        this->logger->debug() << "Player completed a step. New position: (" << step->tileX << ", " << step->tileY << ")";

                        this->mapState->Step(step->tileX, step->tileY);
                    }
                }
            }
        }

        void Scene::ProcessPendingMovement() {
            for (auto& actor : this->actorManager->actors) {
                if (!actor->IsMoving()) {
                    auto nextMove = actor->PeekMovement();

                    if (nextMove.has_value()) {
                        actor->SetDirection(nextMove.value());

                        if (this->mapState->CanMove(actor.get(), nextMove.value())) {
                            actor->PopMovement();
                            actor->StartMovement(nextMove.value());
                        }
                    }
                }
            }
        }
    }
}
