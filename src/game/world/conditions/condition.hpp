#ifndef SDL03_Game_World_ConditionEvaluator
#define SDL03_Game_World_ConditionEvaluator

#include <string>

#include "parser.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            class Condition {
            public:
                Condition(const std::string& condition);
                ~Condition();

                bool Evaluate();

            private:
                std::string condition;
                Parser* parser;
                Expressions::Expression* expression;
            };
        }
    }
}

#endif
