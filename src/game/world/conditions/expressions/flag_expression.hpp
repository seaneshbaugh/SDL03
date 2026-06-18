#ifndef SDL03_Game_World_Conditions_Expressions_FlagExpression
#define SDL03_Game_World_Conditions_Expressions_FlagExpression

#include <string>

#include "expression.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            namespace Expressions {
                class FlagExpression : public Expression {
                public:
                    FlagExpression(const std::string& key);

                    virtual bool Evaluate(const Contexts::Context& context);
                private:
                    std::string key;
                };
            }
        }
    }
}

#endif
