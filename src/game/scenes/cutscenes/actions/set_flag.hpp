#ifndef SDL03_Game_Scenes_Cutscenes_Actions_SetFlag
#define SDL03_Game_Scenes_Cutscenes_Actions_SetFlag

#include "base.hpp"
#include "../../../services/locator.hpp"
#include "../../../world/state.hpp"

namespace Game {
    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                class SetFlag : public Base {
                public:
                    SetFlag(const std::string& key, bool value);
                    ~SetFlag();
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    std::string key;
                    bool value;
                    bool completed;
                };
            }
        }
    }
}

#endif
