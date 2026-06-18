#ifndef SDL03_Game_World_Conditions_Expressions_Expression
#define SDL03_Game_World_Conditions_Expressions_Expression

#include "../contexts/context.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Expressions {
                class Expression {
                public:
                    Expression();
                    virtual ~Expression();

                    virtual bool Evaluate(const Contexts::Context& context) = 0;
                };
            }
        }
    }
}

#endif
