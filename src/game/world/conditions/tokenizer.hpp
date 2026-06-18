#ifndef SDL03_Game_World_Conditions_Tokenizer
#define SDL03_Game_World_Conditions_Tokenizer

#include <sstream>

#include "expressions/flag_expression.hpp"
#include "expressions/negation_expression.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            class Tokenizer {
            public:
                Tokenizer(const std::string& expression);

                Expressions::FlagExpression* Flag();
                Expressions::NegationExpression* Negation();
                bool Character(char expected);
                bool Token(const std::string& expected);
                bool AtEnd();
                int Mark();
                void Reset(int mark);

            private:
                std::stringstream str;

                void SkipWhiteSpace();
            };
        };
    }
}

#endif
