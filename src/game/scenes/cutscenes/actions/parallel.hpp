#ifndef SDL03_Game_Scenes_Cutscenes_Actions_Parallel
#define SDL03_Game_Scenes_Cutscenes_Actions_Parallel

#include <memory>
#include <vector>

#include "base.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                class Parallel : public Base {
                public:
                    Parallel(std::vector<std::shared_ptr<Base>> actions);
                    ~Parallel();
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    std::vector<std::shared_ptr<Base>> actions;
                };
            }
        }
    }
}

#endif
