#include "world_flag_context.hpp"
#include "../../state.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Contexts {
                WorldFlagContext::WorldFlagContext(){
                }

                WorldFlagContext::~WorldFlagContext() {
                }

                bool WorldFlagContext::GetFlag(const std::string& key) const {
                    return Services::Locator::WorldService()->GetState()->flags.Get(key, false);
                }
            }
        }
    }
}
