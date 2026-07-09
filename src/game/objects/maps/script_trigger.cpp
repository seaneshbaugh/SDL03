#include "script_trigger.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            ScriptTrigger::ScriptTrigger(const std::string& scriptId) : scriptId(scriptId) {
            }

            ScriptTrigger::~ScriptTrigger() {
            }

            const std::string& ScriptTrigger::GetScriptId() const {
                return this->scriptId;
            }
        }
    }
}
