#ifndef SDL03_Game_Scene_Cutscenes_Actions_FaceActor
#define SDL03_Game_Scene_Cutscenes_Actions_FaceActor

#include "base.hpp"
#include "../../actor.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                class FaceActor : public Base {
                public:
                    FaceActor(States::Map* map, const std::string& actorId, const Actor::Direction direction);
                    FaceActor(States::Map* map, const std::string& actorId, const std::string& targetId);
                    ~FaceActor();
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;
                private:
                    States::Map* map;
                    std::string actorId;
                    std::string targetId;
                    std::shared_ptr<Scene::Actor> actor;
                    std::shared_ptr<Scene::Actor> target;
                    Actor::Direction direction;
                    bool completed;
                };
            }
        }
    }
}

#endif
