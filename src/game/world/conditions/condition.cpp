#include "condition.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            Condition::Condition() {
            }

            Condition::~Condition() {
            }

            bool Condition::Evaluate(const std::string& condition) {
                Parser parser(condition);

                Expressions::Expression* exp = parser.Expression();

                if (exp) {
                    bool result = exp->Evaluate();

                    delete exp;

                    return result;
                } else {
                    return false;
                }
            }
        }
    }
}
