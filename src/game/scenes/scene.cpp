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

        void Scene::Render() const {
            this->actorManager->RenderActors();
        }
    }
}
