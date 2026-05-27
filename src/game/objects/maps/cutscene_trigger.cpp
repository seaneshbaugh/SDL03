#include "cutscene_trigger.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            CutsceneTrigger::CutsceneTrigger(const std::string& cutsceneId) : cutsceneId(cutsceneId) {
            }

            CutsceneTrigger::~CutsceneTrigger() {
            }

            const std::string& CutsceneTrigger::GetCutsceneId() const {
                return this->cutsceneId;
            }
        }
    }
}
