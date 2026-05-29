#ifndef SDL03_Game_Scene_Cutscenes_Actions_AddActor
#define SDL03_Game_Scene_Cutscenes_Actions_AddActor

#include "base.hpp"
#include "../../../scene/actor.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scene {
        namespace Cutscenes {
            namespace Actions {
                class AddActor : public Base {
                public:
                    AddActor(States::Map* map, const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const int x, const int y, const Scene::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);
                    ~AddActor();
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    States::Map* map;
                    std::string id;
                    std::string name;
                    std::string spritesheetName;
                    std::string dialogueId;
                    std::string spawnPointId;
                    int x;
                    int y;
                    Scene::Actor::Direction direction;
                    std::string movementScriptName;
                    std::string interactionScriptName;
                };
            }
        }
    }
}

#endif
