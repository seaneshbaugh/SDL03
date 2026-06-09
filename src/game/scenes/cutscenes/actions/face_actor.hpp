#ifndef SDL03_Game_Scenes_Cutscenes_Actions_FaceActor
#define SDL03_Game_Scenes_Cutscenes_Actions_FaceActor

#include "base.hpp"
#include "../../scene.hpp"
#include "../../actor.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                class FaceActor : public Base {
                public:
                    FaceActor(Scenes::Scene* scene, const std::string& actorId, const Actor::Direction direction);
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
                    Actor::Direction direction;
                    bool completed;
                };
            }
        }
    }
}

#endif
