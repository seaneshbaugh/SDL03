#include "scene.hpp"
#include "../states/map.hpp"

namespace Game {
    namespace Scenes {
        const std::string Scene::logChannel = "scene";

        Scene::Scene(States::Map* mapState) : mapState(mapState) {
            this->logger = Services::Locator::LoggerService()->GetLogger(Scene::logChannel);
            this->actorManager = std::make_shared<ActorManager>(this->mapState);
            this->pathfinder = std::make_unique<Pathfinder>(this);
        }

        Scene::~Scene() {
        }

        void Scene::Update(const float deltaTime) {
            this->actorManager->UpdateActors(deltaTime);

            this->EnqueueMovement(deltaTime);

            this->ProcessCompletedSteps();

            this->ProcessPendingMovement();
        }

        void Scene::EnqueueMovement(const float deltaTime) {
            for (auto& [id, controller] : this->actorControllers) {
                controller->Update(deltaTime);
            }
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

        void Scene::Render() const {
            this->actorManager->RenderActors();
        }

        std::shared_ptr<Scenes::Actor> Scene::GetActor(const std::string& id) {
            return this->actorManager->GetActor(id);
        }

        // TODO: Make this take an actor definition struct instead of a bunch of parameters.
        template<typename TController> std::shared_ptr<Scenes::Actor> Scene::AddActor(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const int x, const int y, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName) {
            std::shared_ptr<Scenes::Actor> actor = this->actorManager->AddActor(id, name, spritesheetName, dialogueId, x, y, direction, movementScriptName, interactionScriptName);

            std::unique_ptr<TController> actorController = std::make_unique<TController>(actor.get());

            if (this->actorControllers.contains(id)) {
                this->actorControllers.erase(id);
            }

            this->actorControllers[id] = std::move(actorController);

            return actor;
        }

        template std::shared_ptr<Scenes::Actor> Scene::AddActor<Controllers::CutsceneController>(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const int x, const int y, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);
        template std::shared_ptr<Scenes::Actor> Scene::AddActor<Controllers::PlayerController>(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const int x, const int y, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);
        template std::shared_ptr<Scenes::Actor> Scene::AddActor<Controllers::ScriptedController>(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const int x, const int y, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);

        void Scene::RemoveActor(const std::string& id) {
            this->actorManager->RemoveActor(id);

            this->actorControllers.erase(id);
        }

        void Scene::PlaceActor(std::shared_ptr<Scenes::Actor> actor, const int x, const int y, const Scenes::Actor::Direction direction) const {
            this->actorManager->PlaceActor(actor, x, y, direction);
        }

        void Scene::SetActorController(const std::string& id, std::unique_ptr<Controllers::ActorController> controller) {
            this->actorControllers.erase(id);

            this->actorControllers[id] = std::move(controller);
        }

        bool Scene::IsTileBlocked(const int x, const int y, const Scenes::Actor* ignore) const {
            for (auto& actor : this->actorManager->actors) {
                if (actor.get() == ignore) {
                    continue;
                }

                if (actor->OccupiesTile(x, y)) {
                    return true;
                }
            }

            return false;
        }

        void Scene::PathfindActor(const std::string& actorId, const int targetX, const int targetY) {
            std::shared_ptr<Scenes::Actor> actor = this->GetActor(actorId);

            if (!actor) {
                return;
            }

            this->PathfindActor(actor.get(), targetX, targetY);
        }

        void Scene::PathfindActor(Actor* actor, const int targetX, const int targetY) {
            std::vector<Actor::Direction> path = this->pathfinder->Pathfind(actor, targetX, targetY);

            for (auto direction : path) {
                actor->QueueMovement(direction);
            }
        }

        Objects::Maps::Map* Scene::GetCurrentMap() const {
            return this->mapState->currentMap.get();
        }
    }
}
