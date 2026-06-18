#ifndef SDL03_Game_World_Conditions_Expressions_NegationExpression
#define SDL03_Game_World_Conditions_Expressions_NegationExpression

#include "expression.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Expressions {
                class NegationExpression : public Expression {
                public:
                    NegationExpression(Expression* operand);
                    virtual ~NegationExpression();

                    virtual bool Evaluate(const Contexts::Context& context);

                private:
                    Expression* operand;
                };
            }
        }
    }
}

#endif
