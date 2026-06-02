#ifndef SDL03_Game_Scene_Cutscenes_Actions_Sequence
#define SDL03_Game_Scene_Cutscenes_Actions_Sequence

#include <memory>
#include <vector>

#include "base.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                class Sequence : public Base {
                public:
                    Sequence(std::vector<std::shared_ptr<Base>> actions);
                    ~Sequence();
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    std::vector<std::shared_ptr<Base>> actions;
                    size_t currentActionIndex;
                };
            }
        }
    }
}

#endif
