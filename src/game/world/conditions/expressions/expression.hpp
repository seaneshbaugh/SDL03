#ifndef SDL03_Game_World_Conditions_Expressions_Expression
#define SDL03_Game_World_Conditions_Expressions_Expression

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Expressions {
                class Expression {
                public:
                    Expression();
                    virtual ~Expression();

                    virtual bool Evaluate() = 0;
                };
            }
        }
    }
}

#endif
