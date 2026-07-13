#ifndef SDL03_Game_Objects_Maps_ScriptTrigger
#define SDL03_Game_Objects_Maps_ScriptTrigger

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
