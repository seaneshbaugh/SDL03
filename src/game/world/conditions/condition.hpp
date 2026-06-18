#ifndef SDL03_Game_World_ConditionEvaluator
#define SDL03_Game_World_ConditionEvaluator

#include <string>

#include "parser.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            class Condition {
            public:
                Condition();
                ~Condition();
                static bool Evaluate(const std::string& condition);
            };
        }
    }
}

#endif
