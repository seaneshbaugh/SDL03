#include "flag_expression.hpp"
#include "../../../services/locator.hpp"
#include "../../state.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Expressions {
                 FlagExpression::FlagExpression(const std::string& key) : key(key) {
                 }

                 bool FlagExpression::Evaluate(const EvaluationContexts::EvaluationContext& context) {
                     return context.GetFlag(this->key);
                 }

            }
        }
    }
}
