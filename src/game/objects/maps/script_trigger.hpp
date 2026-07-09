#ifndef SDL03_Game_Objects_Maps_CutsceneTrigger
#define SDL03_Game_Objects_Maps_CutsceneTrigger

#include "map_object.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            class ScriptTrigger : public MapObject {
            public:
                ScriptTrigger(const std::string& scriptId);
                ~ScriptTrigger();
                const std::string& GetScriptId() const;
            private:
                std::string scriptId;
            };
        }
    }
}

#endif
