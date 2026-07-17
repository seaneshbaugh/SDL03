#include "scene.hpp"
#include "../states/map.hpp"

namespace Game {
    namespace Scenes {
        const std::string Scene::logChannel = "scene";

        Scene::Scene(std::shared_ptr<Objects::Maps::Map> currentMap, Interfaces::CommandQueue* commandQueue) : currentMap(currentMap), commandQueue(commandQueue) {
            this->logger = Services::Locator::LoggerService()->GetLogger(Scene::logChannel);
            this->actorManager = std::make_shared<ActorManager>(this);
            this->camera = std::make_shared<Scenes::Camera>(0.0f, 0.0f, static_cast<float>(Services::Locator::VideoService()->GetScreenWidth()), static_cast<float>(Services::Locator::VideoService()->GetScreenHeight()));
            this->pathfinder = std::make_unique<Pathfinder>(this);
            this->worldEvaluationContext = std::make_unique<World::Conditions::EvaluationContexts::WorldEvaluationContext>();
        }

        Scene::~Scene() {
        }

        std::shared_ptr<Objects::Maps::Map> Scene::GetCurrentMap() const {
            return this->currentMap;
        }

        void Scene::SetCurrentMap(std::shared_ptr<Objects::Maps::Map> map) {
            this->currentMap = map;
        }

        void Scene::Update(const float deltaTime) {
            this->actorManager->UpdateActors(deltaTime);

            this->EnqueueMovement(deltaTime);

            this->ProcessCompletedSteps();

            this->ProcessPendingMovement();

            this->ProcessInteractions();

            this->camera->Update(deltaTime, this->currentMap->width * this->currentMap->tilewidth, this->currentMap->height * this->currentMap->tileheight);
        }

        void Scene::EnqueueMovement(const float deltaTime) {
            for (auto& [id, controller] : this->actorControllers) {
                controller->Update(deltaTime);
            }
        }

        void Scene::ProcessCompletedSteps() {
            for (auto& actor : this->actorManager->actors) {
                while (auto step = actor->ConsumeCompletedStep()) {
                    // TODO: Remove this check to see if the actor is the player. Eventually it shouldn't needed.
                    if (actor == this->actorManager->player) {
                        Services::Locator::WorldService()->UpdatePlayerPosition(step->tileX, step->tileY);

                        this->logger->debug() << "Player completed a step. New position: (" << step->tileX << ", " << step->tileY << ")";

                        this->OnActorStepped(actor.get(), step->tileX, step->tileY);
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

                        if (this->CanMove(actor.get(), nextMove.value())) {
                            actor->PopMovement();
                            actor->StartMovement(nextMove.value());
                        }
                    }
                }
            }
        }

        void Scene::ProcessInteractions() const {
            for (auto& actor : this->actorManager->actors) {
                if (actor->PeekInteraction()) {
                    actor->ConsumeInteraction();

                    int targetX = actor->GetOccupiedTileX();
                    int targetY = actor->GetOccupiedTileY();

                    switch (actor->GetDirection()) {
                    case Direction::Up:
                        targetY--;

                        break;
                    case Direction::Right:
                        targetX++;

                        break;
                    case Direction::Down:
                        targetY++;

                        break;
                    case Direction::Left:
                        targetX--;

                        break;
                    }

                    if (targetX < 0 || targetX >= this->currentMap->width || targetY < 0 || targetY >= this->currentMap->height) {
                        continue;
                    }

                    auto target = this->GetActorAtTile(targetX, targetY);

                    if (target.has_value()) {
                        if (target.value().get() == actor.get()) {
                            continue;
                        }

                        actor->Interact(target.value());
                    }

                    // std::vector<std::shared_ptr<Objects::Maps::MapObject>> objects = this->currentMap->GetObjects(targetX, targetY);

                    // for (auto object = objects.begin(); object != objects.end(); object++) {
                    //     if ((*object)->GetType() == "interactable") {
                    //         (*this->luaState.get())["on_interact"](*object);
                    //         return true;
                    //     }
                    // }
                }
            }
        }

        void Scene::Render() const {
            this->actorManager->RenderActors();
        }

        std::shared_ptr<Scenes::Actor> Scene::GetActor(const std::string& id) {
            return this->actorManager->GetActor(id);
        }

        std::optional<std::shared_ptr<Scenes::Actor>> Scene::GetActorAtTile(const int x, const int y) const {
            for (auto& actor : this->actorManager->actors) {
                if (actor->OccupiesTile(x, y)) {
                    return actor;
                }
            }

            return std::nullopt;
        }

        // TODO: Make this take an actor definition struct instead of a bunch of parameters.
        template<typename TController> std::shared_ptr<Scenes::Actor> Scene::AddActor(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueProfileId, const int x, const int y, const Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName) {
            std::shared_ptr<Scenes::Actor> actor = this->actorManager->AddActor(id, name, spritesheetName, dialogueProfileId, x, y, direction, movementScriptName, interactionScriptName);

            std::unique_ptr<TController> actorController = std::make_unique<TController>(actor.get());

            if (this->actorControllers.contains(id)) {
                this->actorControllers.erase(id);
            }

            this->actorControllers[id] = std::move(actorController);

            return actor;
        }

        template std::shared_ptr<Scenes::Actor> Scene::AddActor<Controllers::NullController>(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueProfileId, const int x, const int y, const Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);
        template std::shared_ptr<Scenes::Actor> Scene::AddActor<Controllers::PlayerController>(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueProfileId, const int x, const int y, const Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);
        template std::shared_ptr<Scenes::Actor> Scene::AddActor<Controllers::AIController>(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueProfileId, const int x, const int y, const Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);

        void Scene::RemoveActor(const std::string& id) {
            this->actorManager->RemoveActor(id);

            this->actorControllers.erase(id);
        }

        void Scene::PlaceActor(std::shared_ptr<Scenes::Actor> actor, const int x, const int y, const Direction direction) const {
            this->actorManager->PlaceActor(actor, x, y, direction);
        }

        void Scene::SetActorController(const std::string& id, std::unique_ptr<Controllers::ActorController> controller) {
            this->actorControllers.erase(id);

            this->actorControllers[id] = std::move(controller);
        }

        void Scene::PathfindActor(const std::string& actorId, const int targetX, const int targetY) {
            std::shared_ptr<Scenes::Actor> actor = this->GetActor(actorId);

            if (!actor) {
                return;
            }

            this->PathfindActor(actor.get(), targetX, targetY);
        }

        void Scene::PathfindActor(Actor* actor, const int targetX, const int targetY) {
            std::vector<Direction> path = this->pathfinder->Pathfind(actor, targetX, targetY);

            for (auto direction : path) {
                actor->QueueMovement(direction);
            }
        }

        bool Scene::CanMove(Scenes::Actor* actor, const Direction direction) const {
            int targetX = actor->GetCurrentTileX();
            int targetY = actor->GetCurrentTileY();

            switch (direction) {
            case Direction::Up:
                targetY--;

                break;
            case Direction::Right:
                targetX++;

                break;
            case Direction::Down:
                targetY++;

                break;
            case Direction::Left:
                targetX--;

                break;
            }

            if (targetX < 0 || targetX >= this->currentMap->width || targetY < 0 || targetY >= this->currentMap->height) {
                return false;
            }

            if (!this->currentMap->GetWalkability(targetX, targetY)) {
                return false;
            }

            if (this->IsTileBlocked(targetX, targetY, actor)) {
                return false;
            }

            return true;
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

        void Scene::OnActorStepped(Scenes::Actor* actor, unsigned int x, unsigned int y) {
            auto objects = this->currentMap->GetObjects(x, y);

            for (auto object = objects.begin(); object != objects.end(); object++) {
                if (!(*object)->IsConditionSatisfied(*(this->worldEvaluationContext.get()))) {
                    continue;
                }

                Objects::Maps::MapLoadPoint* mapLoadPoint = dynamic_cast<Objects::Maps::MapLoadPoint*>(object->get());

                if (mapLoadPoint) {
                    // TODO: Make the MapLoadPoint parser handle this.
                    const int startX = mapLoadPoint->GetProperty("x") != "" ? std::stoi(mapLoadPoint->GetProperty("x")) : 0;
                    const int startY = mapLoadPoint->GetProperty("y") != "" ? std::stoi(mapLoadPoint->GetProperty("y")) : 0;

                    this->commandQueue->QueueCommand(States::LoadMapCommand{mapLoadPoint->GetProperty("map"), startX, startY});

                    break;
                }

                Objects::Maps::ScriptTrigger* scriptTrigger = dynamic_cast<Objects::Maps::ScriptTrigger*>(object->get());

                if (scriptTrigger) {
                    this->logger->debug() << "Player stepped on a script trigger with script ID \"" << scriptTrigger->GetScriptId() << "\".";

                    this->commandQueue->QueueCommand(States::StartScriptCommand{scriptTrigger->GetScriptId()});

                    break;
                }
            }
        }

        World::Conditions::EvaluationContexts::WorldEvaluationContext& Scene::GetWorldEvaluationContext() const {
            return *(this->worldEvaluationContext.get());
        }
    }
}
