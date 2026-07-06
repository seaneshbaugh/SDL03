#include "world_evaluation_context.hpp"
#include "../../state.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace EvaluationContexts {
                WorldEvaluationContext::WorldEvaluationContext(){
                }

                WorldEvaluationContext::~WorldEvaluationContext() {
                }

                bool WorldEvaluationContext::GetFlag(const std::string& key) const {
                    return Services::Locator::WorldService()->GetState()->flags.Get(key, false);
                }
            }
        }
    }
}
