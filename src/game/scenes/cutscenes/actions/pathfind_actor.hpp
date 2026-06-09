#ifndef SDL03_Game_Scenes_Cutscenes_Actions_PathfindActor
#define SDL03_Game_Scenes_Cutscenes_Actions_PathfindActor

#include "base.hpp"
#include "../../scene.hpp"
#include "../../actor.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                class PathfindActor : public Base {
                public:
                    PathfindActor(Scenes::Scene* scene, const std::string& actorId, const int targetX, const int targetY);
                    PathfindActor(Scenes::Scene* scene, const std::string& actorId, const std::string& targetId);
                    ~PathfindActor();
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    Scenes::Scene* scene;
                    bool started;
                    bool failed;
                    std::string actorId;
                    std::shared_ptr<Scenes::Actor> actor;
                    int targetX;
                    int targetY;
                    std::string targetId;
                    std::shared_ptr<Scenes::Actor> target;
                };
            }
        }
    }
}

#endif
