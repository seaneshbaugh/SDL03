#ifndef SDL03_Game_Scene_Cutscenes_Actions_MoveActor
#define SDL03_Game_Scene_Cutscenes_Actions_MoveActor

#include "base.hpp"
#include "../../actor.hpp"

namespace Game {
    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                class MoveActor : public Base {
                public:
                    MoveActor(std::shared_ptr<Actor> actor, const std::vector<Actor::Direction>& path);
                    virtual ~MoveActor() = default;
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    std::shared_ptr<Actor> actor;
                    std::vector<Actor::Direction> path;
                    bool started;
                };
            }
        }
    }
}

#endif
