#ifndef SDL03_Game_Actions_FaceActor
#define SDL03_Game_Actions_FaceActor

#include "../types.hpp"
#include "../scenes/actor.hpp"
#include "../scenes/scene.hpp"
#include "base.hpp"

namespace Game {
    namespace Actions {
        class FaceActor : public Base {
        public:
            FaceActor(Scenes::Scene* scene, const std::string& actorId, const Direction direction);
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
            Direction direction;
            bool completed;
        };
    }
}

#endif
