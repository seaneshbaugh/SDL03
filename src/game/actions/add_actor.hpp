#ifndef SDL03_Game_Actions_AddActor
#define SDL03_Game_Actions_AddActor

#include "base.hpp"
#include "../scenes/scene.hpp"
#include "../scenes/actor.hpp"

namespace Game {
    namespace Actions {
        class AddActor : public Base {
        public:
            AddActor(Scenes::Scene* scene, const std::string& id, const std::string& name, const std::string& spritesheetName, const std::string& dialogueId, const int x, const int y, const Scenes::Actor::Direction direction, const std::string& movementScriptName, const std::string& interactionScriptName);
            ~AddActor();
            void Start() override;
            void Update(float deltaTime) override;
            bool IsCompleted() const override;

        private:
            Scenes::Scene* scene;
            std::string id;
            std::string name;
            std::string spritesheetName;
            std::string dialogueId;
            std::string spawnPointId;
            int x;
            int y;
            Scenes::Actor::Direction direction;
            std::string movementScriptName;
            std::string interactionScriptName;
        };
    }
}

#endif
