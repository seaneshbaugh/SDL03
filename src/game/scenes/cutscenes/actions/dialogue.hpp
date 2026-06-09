#ifndef SDL03_Game_Scenes_Cutscenes_Actions_Dialogue
#define SDL03_Game_Scenes_Cutscenes_Actions_Dialogue

#include <memory>

#include "base.hpp"

namespace Game {
    namespace States {
        class Map;
    }

    namespace Scenes {
        namespace Cutscenes {
            namespace Actions {
                class Dialogue : public Base {
                public:
                    Dialogue(States::Map* map, const std::string& dialogueId);
                    virtual ~Dialogue() = default;
                    void Start() override;
                    void Update(float deltaTime) override;
                    bool IsCompleted() const override;

                private:
                    States::Map* map;
                    std::string dialogueId;
                    bool started;
                };
            }
        }
    }
}

#endif
