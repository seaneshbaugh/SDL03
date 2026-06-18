#ifndef SDL03_Game_World_Conditions_Contexts_Context
#define SDL03_Game_World_Conditions_Contexts_Context

#include <string>

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Contexts {
                class Context {
                public:
                    virtual ~Context() = default;

                    virtual bool GetFlag(const std::string& key) const = 0;
                };
            }
        }
    }
}

#endif
