#include "actor_manager.hpp"

namespace Game {
    namespace Scene {
        const std::string ActorManager::logChannel = "scene";

        ActorManager::ActorManager() {
            this->logger = Services::Locator::LoggerService()->GetLogger(ActorManager::logChannel);
        }

        ActorManager::~ActorManager() {
        }

        std::shared_ptr<Scene::Actor> ActorManager::GetActor(const std::string& actorId) {
            auto it = this->actorLookup.find(actorId);

            if (it != this->actorLookup.end()) {
                return it->second;
            }

            this->logger->warning() << "Failed to get actor with ID \"" << actorId << "\".";

            return nullptr;
        }

        void ActorManager::RemoveActor(const std::string& actorId) {
            auto actor = this->GetActor(actorId);

            if (actor) {
                this->actors.erase(std::remove(this->actors.begin(), this->actors.end(), actor), this->actors.end());
                this->actorLookup.erase(actorId);
            }
        }
    }
}
