#ifndef SDL03_Game_Scene_Cutscenes_Actions_MoveActor
#define SDL03_Game_Scene_Cutscenes_Actions_MoveActor

#include "base.hpp"
#include "../../scene.hpp"
#include "../../actor.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                class MoveActor : public Base {
                public:
                    MoveActor(Scenes::Scene* scene, const std::string& actorId, const std::vector<Actor::Direction>& path);
                    virtual ~MoveActor() = default;
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    Scenes::Scene* scene;
                    std::string actorId;
                    std::shared_ptr<Actor> actor;
                    std::vector<Actor::Direction> path;
                    bool started;
                    bool failed;
                };
            }
        }
    }
}

#endif
