#ifndef SDL03_Game_World_Conditions_EvaluationContexts_WorldEvaluationContext
#define SDL03_Game_World_Conditions_EvaluationContexts_WorldEvaluationContext

#include "evaluation_context.hpp"
#include "../../../services/locator.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace EvaluationContexts {
                class WorldEvaluationContext : public EvaluationContext {
                public:
                    WorldEvaluationContext();
                    virtual ~WorldEvaluationContext();

                    virtual bool GetFlag(const std::string& key) const override;
                };
            }
        }
    }
}

#endif
