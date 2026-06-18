#ifndef SDL03_Game_World_Conditions_Expressions_LogicalExpression
#define SDL03_Game_World_Conditions_Expressions_LogicalExpression

#include <string>

#include "expression.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Expressions {
                class LogicalExpression : public Expression {
                public:
                    LogicalExpression(const std::string& operation, Expression* left, Expression* right);
                    virtual ~LogicalExpression();

                    virtual bool Evaluate(const Contexts::Context& context);

                private:
                    std::string operation;
                    Expression* left;
                    Expression* right;
                };
            }
        }
    }
}

#endif
