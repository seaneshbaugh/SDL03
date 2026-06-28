#include "actor_manager.hpp"
#include "scene.hpp"

namespace Game {
    namespace Scenes {
        const std::string ActorManager::logChannel = "scene";

        ActorManager::ActorManager(Scene* scene) : scene(scene) {
            this->logger = Services::Locator::LoggerService()->GetLogger(ActorManager::logChannel);
        }

        ActorManager::~ActorManager() {
        }

        std::shared_ptr<Scenes::Actor> ActorManager::GetActor(const std::string& id) const {
            auto it = this->actorLookup.find(id);

            if (it != this->actorLookup.end()) {
                return it->second;
            }

            this->logger->warning() << "Failed to get actor with ID \"" << id << "\".";

            return nullptr;
        }

        std::shared_ptr<Scenes::Actor> ActorManager::AddActor(const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueProfileId, const int x, const int y, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName) {
            std::shared_ptr<Graphics::Spritesheet> spritesheet = std::make_shared<Graphics::Spritesheet>(spritesheetName);

            std::shared_ptr<Scenes::Actor> actor = std::make_shared<Scenes::Actor>(spritesheet);

            actor->id = id;
            actor->name = name;
            actor->dialogueSelector = std::make_unique<Dialogue::DialogueSelector>(dialogueProfileId);
            actor->SetMovementSpeed(2.0f);
            actor->SetCommandQueue(this->scene->commandQueue);

            if (!movementScriptName.empty()) {
                actor->LoadLuaScript("scripts/actors/movement/" + movementScriptName + ".lua");
            }

            if (!interactionScriptName.empty()) {
                actor->LoadLuaScript("scripts/actors/interaction/" + interactionScriptName + ".lua");
            }

            if (this->actorLookup.contains(id)) {
                this->RemoveActor(id);
            }

            this->actors.push_back(actor);

            this->actorLookup[actor->id] = actor;

            this->PlaceActor(actor, x, y, direction);

            return actor;
        }

        void ActorManager::RemoveActor(const std::string& id) {
            auto actor = this->GetActor(id);

            if (actor) {
                this->actors.erase(std::remove(this->actors.begin(), this->actors.end(), actor), this->actors.end());
                this->actorLookup.erase(id);
            }
        }

        void ActorManager::PlaceActor(std::shared_ptr<Scenes::Actor> actor, const int x, const int y, const Scenes::Actor::Direction direction) const {
            actor->currentMap = this->scene->GetCurrentMap();
            actor->SetPosition(x, y);
            actor->SetAnimation(Scenes::Actor::Animation::Stand);
            actor->SetDirection(direction);
            actor->animationFrame = 0;
            actor->timeSinceLastAnimationFrame = 0.0f;
        }

        void ActorManager::UpdateActors(const float deltaTime) {
            for (auto& actor : this->actors) {
                actor->Update(deltaTime);
            }
        }

        void ActorManager::RenderActors() {
            std::vector<Scenes::Actor*> renderActors;

            renderActors.reserve(this->actors.size());

            for (auto& actor : this->actors) {
                renderActors.push_back(actor.get());
            }

            std::sort(renderActors.begin(), renderActors.end(), [](const Scenes::Actor* a, const Scenes::Actor* b) {
                if (a->GetOccupiedTileY() == b->GetOccupiedTileY()) {
                    return a->GetOccupiedTileX() < b->GetOccupiedTileX();
                }

                return a->GetOccupiedTileY() < b->GetOccupiedTileY();
            });

            for (auto& actor : renderActors) {
                actor->Render(this->scene->camera);
            }
        }
    }
}
