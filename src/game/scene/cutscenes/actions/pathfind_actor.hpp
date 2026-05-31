#ifndef SDL03_Game_Scene_Cutscenes_Actions_PathfindActor
#define SDL03_Game_Scene_Cutscenes_Actions_PathfindActor

#include "base.hpp"
#include "../../actor.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                class PathfindActor : public Base {
                public:
                    PathfindActor(States::Map* map, const std::string& actorId, const int targetX, const int targetY);
                    PathfindActor(States::Map* map, const std::string& actorId, const std::string& targetId);
                    ~PathfindActor();
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    States::Map* map;
                    bool started;
                    std::string actorId;
                    std::shared_ptr<Scene::Actor> actor;
                    int targetX;
                    int targetY;
                    std::string targetId;
                    std::shared_ptr<Scene::Actor> target;
                    std::vector<Scene::Actor::Direction> path;
                };
            }
        }
    }
}

#endif
