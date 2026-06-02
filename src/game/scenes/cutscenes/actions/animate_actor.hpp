#ifndef SDL03_Game_Scene_Cutscenes_Actions_AnimateActor
#define SDL03_Game_Scene_Cutscenes_Actions_AnimateActor

#include "base.hpp"
#include "../../actor.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                class AnimateActor : public Base {
                public:
                    AnimateActor(States::Map* map, const std::string& actorId, const std::string& animationName, float duration);
                    ~AnimateActor();
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    States::Map* map;
                    std::string actorId;
                    std::string animationName;
                    std::shared_ptr<Scenes::Actor> actor;
                    float duration;
                    float elapsedTime;
                    bool started;
                    bool failed;
                    Actor::Animation previousAnimation;
                    bool previousAnimationRestored;
                };
            }
        }
    }
}

#endif
