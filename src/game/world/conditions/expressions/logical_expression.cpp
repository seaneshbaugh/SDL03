#include "logical_expression.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Expressions {
                LogicalExpression::LogicalExpression(const std::string& operation, Expression* left, Expression* right) : operation(operation), left(left), right(right) {
                }

                LogicalExpression::~LogicalExpression() {
                    if (this->left) {
                        delete this->left;
                    }

                    if (this->right) {
                        delete this->right;
                    }
                }

                bool LogicalExpression::Evaluate(const EvaluationContexts::EvaluationContext& context) {
                    if (this->left == nullptr || this->right == nullptr) {
                        return false;
                    }

                    if (this->operation == "&&") {
                        return this->left->Evaluate(context) && this->right->Evaluate(context);
                    } else if (this->operation == "||") {
                        return this->left->Evaluate(context) || this->right->Evaluate(context);
                    } else {
                        return false;
                    }
                }
            }
        }
    }
}
