#ifndef SDL03_Game_Scene_Cutscenes_Actions_RemoveActor
#define SDL03_Game_Scene_Cutscenes_Actions_RemoveActor

#include "base.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                class RemoveActor : public Base {
                public:
                    RemoveActor(States::Map* map, const std::string& actorId);
                    ~RemoveActor();
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;
 
                private:
                    States::Map* map;
                    std::string actorId;
                };
            }
        }
    }
}

#endif
