#ifndef SDL03_Game_World_ConditionEvaluator
#define SDL03_Game_World_ConditionEvaluator

#include <string>
#include <sstream>

#include "../services/locator.hpp"

namespace Game {
    namespace World {
        class ConditionEvaluator {
        public:
            ConditionEvaluator();
            ~ConditionEvaluator();
            static bool Evaluate(const std::string& condition);

            class Expression {
            public:
                Expression();
                virtual ~Expression();

                virtual bool Evaluate() = 0;
            };

            class FlagExpression : public Expression {
            public:
                FlagExpression(const std::string& key);

                virtual bool Evaluate();

            private:
                std::string key;
            };

            class NegationExpression : public Expression {
            public:
                NegationExpression(Expression* operand);
                virtual ~NegationExpression();
                virtual bool Evaluate();

            private:
                Expression* operand;
            };

            class LogicalExpression : public Expression {
            public:
                LogicalExpression(const std::string& operation, Expression* left, Expression* right);
                virtual ~LogicalExpression();

                virtual bool Evaluate();

            private:
                std::string operation;
                Expression* left;
                Expression* right;
            };

            class Tokenizer {
            public:
                Tokenizer(const std::string& expression);

                FlagExpression* Flag();
                NegationExpression* Negation();
                bool Character(char expected);
                bool Token(const std::string& expected);
                bool AtEnd();
                int Mark();
                void Reset(int mark);
            private:
                std::stringstream str;

                void SkipWhiteSpace();
            };

            class Parser {
            public:
                Parser(const std::string& expression);
                ~Parser();

                ConditionEvaluator::Expression* Expression();

            private:
                Tokenizer* tokens;

                ConditionEvaluator::Expression* Or();
                ConditionEvaluator::Expression* And();
                ConditionEvaluator::Expression* Unary();
                ConditionEvaluator::Expression* Primary();
            };
        };
    }
}

#endif
