#ifndef SDL03_Game_World_Conditions_Parser
#define SDL03_Game_World_Conditions_Parser

#include <string>

#include "expressions/flag_expression.hpp"
#include "expressions/logical_expression.hpp"
#include "expressions/negation_expression.hpp"
#include "tokenizer.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            class Parser {
            public:
                Parser(const std::string& expression);
                ~Parser();

                Expressions::Expression* Expression();

            private:
                Tokenizer* tokens;

                Expressions::Expression* Or();
                Expressions::Expression* And();
                Expressions::Expression* Unary();
                Expressions::Expression* Primary();
            };
        };
    }
}

#endif
