#ifndef SDL03_Game_World_Conditions_Contexts_WorldFlagContext
#define SDL03_Game_World_Conditions_Contexts_WorldFlagContext

#include "context.hpp"
#include "../../../services/locator.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Contexts {
                class WorldFlagContext : public Context {
                public:
                    WorldFlagContext();
                    virtual ~WorldFlagContext();

                    virtual bool GetFlag(const std::string& key) const override;
                };
            }
        }
    }
}

#endif
