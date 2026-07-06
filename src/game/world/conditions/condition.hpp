#ifndef SDL03_Game_World_ConditionEvaluator
#define SDL03_Game_World_ConditionEvaluator

#include <string>

#include "parser.hpp"
#include "evaluation_contexts/evaluation_context.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            class Condition {
            public:
                Condition(const std::string& condition);
                ~Condition();

                bool Evaluate(const EvaluationContexts::EvaluationContext& context);

            private:
                std::string condition;
                Parser* parser;
                Expressions::Expression* expression;
            };
        }
    }
}

#endif
