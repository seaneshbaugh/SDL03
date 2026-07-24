#ifndef SDL03_Game_Actions_AnimateActor
#define SDL03_Game_Actions_AnimateActor

#include "base.hpp"
#include "../scenes/scene.hpp"
#include "../scenes/actor.hpp"

namespace Game {
    namespace Actions {
        class AnimateActor : public Base {
        public:
            AnimateActor(Scenes::Scene* scene, const std::string& actorId, const std::string& animationName, int loops);
            ~AnimateActor();
            void Start() override;
            void Update(float deltaTime) override;
            bool IsCompleted() const override;

        private:
            Scenes::Scene* scene;
            std::string actorId;
            std::string animationName;
            std::shared_ptr<Scenes::Actor> actor;
            int loops;
            int currentLoop;
            std::shared_ptr<Graphics::AnimationClip> previousAnimationClip;
            bool started;
            bool failed;
        };
    }
}

#endif
