#ifndef SDL03_Game_World_Conditions_Expressions_Expression
#define SDL03_Game_World_Conditions_Expressions_Expression

#include "../evaluation_contexts/evaluation_context.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Expressions {
                class Expression {
                public:
                    Expression();
                    virtual ~Expression();

                    virtual bool Evaluate(const EvaluationContexts::EvaluationContext& context) = 0;
                };
            }
        }
    }
}

#endif
