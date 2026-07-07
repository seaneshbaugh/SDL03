#ifndef SDL03_Game_Actions_RemoveActor
#define SDL03_Game_Actions_RemoveActor

#include "../scenes/scene.hpp"
#include "base.hpp"

namespace Game {
    namespace Actions {
        class RemoveActor : public Base {
        public:
            RemoveActor(Scenes::Scene* scene, const std::string& actorId);
            ~RemoveActor();
            void Start() override;
            void Update(float deltaTime) override;
            bool IsCompleted() const override;
 
        private:
            Scenes::Scene* scene;
            bool completed;
            std::string actorId;
        };
    }
}

#endif
