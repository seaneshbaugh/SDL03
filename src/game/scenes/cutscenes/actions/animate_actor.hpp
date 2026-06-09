#ifndef SDL03_Game_Scenes_Cutscenes_Actions_AnimateActor
#define SDL03_Game_Scenes_Cutscenes_Actions_AnimateActor

#include "base.hpp"
#include "../../scene.hpp"
#include "../../actor.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                class AnimateActor : public Base {
                public:
                    AnimateActor(Scenes::Scene* scene, const std::string& actorId, const std::string& animationName, float duration);
                    ~AnimateActor();
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    Scenes::Scene* scene;
                    std::string actorId;
                    std::string animationName;
                    std::shared_ptr<Scenes::Actor> actor;
                    float duration;
                    float elapsedTime;
                    Actor::Animation previousAnimation;
                    bool previousAnimationRestored;
                    bool started;
                    bool failed;
                };
            }
        }
    }
}

#endif
