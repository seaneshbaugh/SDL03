#ifndef SDL03_Game_Actions_FaceActor
#define SDL03_Game_Actions_FaceActor

#include "base.hpp"
#include "../scenes/scene.hpp"
#include "../scenes/actor.hpp"

namespace Game {
    namespace Actions {
        class FaceActor : public Base {
        public:
            FaceActor(Scenes::Scene* scene, const std::string& actorId, const Scenes::Actor::Direction direction);
            FaceActor(Scenes::Scene* scene, const std::string& actorId, const std::string& targetId);
            ~FaceActor();
            void Start() override;
            void Update(float deltaTime) override;
            bool IsCompleted() const override;
        private:
            Scenes::Scene* scene;
            std::string actorId;
            std::string targetId;
            std::shared_ptr<Scenes::Actor> actor;
            std::shared_ptr<Scenes::Actor> target;
            Scenes::Actor::Direction direction;
            bool completed;
        };
    }
}

#endif
