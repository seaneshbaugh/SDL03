#ifndef SDL03_Game_Objects_Maps_CutsceneTrigger
#define SDL03_Game_Objects_Maps_CutsceneTrigger

#include "map_object.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            class CutsceneTrigger : public MapObject {
            public:
                CutsceneTrigger(const std::string& cutsceneId);
                ~CutsceneTrigger();
                const std::string& GetCutsceneId() const;
            private:
                std::string cutsceneId;
            };
        }
    }
}

#endif
