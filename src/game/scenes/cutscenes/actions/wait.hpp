#ifndef SDL03_Game_Scene_Cutscenes_Actions_Wait
#define SDL03_Game_Scene_Cutscenes_Actions_Wait

#include "base.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                class Wait : public Base {
                public:
                    Wait(float duration);
                    virtual ~Wait() = default;
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    float duration;
                    float elapsed;
                };
            }
        }
    }
}

#endif
