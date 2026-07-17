#ifndef SDL03_Game_Actions_MoveActor
#define SDL03_Game_Actions_MoveActor

#include "../types.hpp"
#include "../scenes/actor.hpp"
#include "../scenes/scene.hpp"
#include "base.hpp"

namespace Game {
    namespace Actions {
        class MoveActor : public Base {
        public:
            MoveActor(Scenes::Scene* scene, const std::string& actorId, const std::vector<Direction>& path);
            virtual ~MoveActor() = default;
            void Start() override;
            void Update(float deltaTime) override;
            bool IsCompleted() const override;

        private:
            Scenes::Scene* scene;
            std::string actorId;
            std::shared_ptr<Scenes::Actor> actor;
            std::vector<Direction> path;
            bool started;
            bool failed;
        };
    }
}

#endif
