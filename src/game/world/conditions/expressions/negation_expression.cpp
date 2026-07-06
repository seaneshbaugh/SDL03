#include "negation_expression.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Expressions {
                NegationExpression::NegationExpression(Expression* operand) : operand(operand) {
                }

                NegationExpression::~NegationExpression() {
                    if (this->operand) {
                        delete this->operand;
                    }
                }

                bool NegationExpression::Evaluate(const EvaluationContexts::EvaluationContext& context) {
                    if (this->operand == nullptr) {
                        return false;
                    }

                    return !this->operand->Evaluate(context);
                }
            }
        }
    }
}
