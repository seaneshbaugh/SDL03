#ifndef SDL03_Game_World_Conditions_EvaluationContexts_EvaluationContext
#define SDL03_Game_World_Conditions_EvaluationContexts_EvaluationContext

#include <string>

namespace Game {
    namespace World {
        namespace Conditions {
            namespace EvaluationContexts {
                class EvaluationContext {
                public:
                    virtual ~EvaluationContext() = default;

                    virtual bool GetFlag(const std::string& key) const = 0;
                };
            }
        }
    }
}

#endif
