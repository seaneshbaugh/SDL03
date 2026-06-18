#include "condition_evaluator.hpp"
#include "state.hpp"

namespace Game {
    namespace World {
        ConditionEvaluator::ConditionEvaluator() {
        }

        ConditionEvaluator::~ConditionEvaluator() {
        }

        bool ConditionEvaluator::Evaluate(const std::string& condition) {
            World::ConditionEvaluator::Parser parser(condition);

            ConditionEvaluator::Expression* exp = parser.Expression();

            if (exp) {
                bool result = exp->Evaluate();

                delete exp;

                return result;
            } else {
                return false;
            }
        }

        ConditionEvaluator::Expression::Expression() {
        }

        ConditionEvaluator::Expression::~Expression() {
        }

        ConditionEvaluator::FlagExpression::FlagExpression(const std::string& key) : key(key) {
        }

        bool ConditionEvaluator::FlagExpression::Evaluate() {
            return Services::Locator::WorldService()->GetState()->flags.Get(key, false);
        }

        ConditionEvaluator::NegationExpression::NegationExpression(Expression* operand) : operand(operand) {
        }

        ConditionEvaluator::NegationExpression::~NegationExpression() {
            if (operand) {
                delete operand;
            }
        }

        bool ConditionEvaluator::NegationExpression::Evaluate() {
            if (operand == nullptr) {
                return false;
            }

            return !operand->Evaluate();
        }

        ConditionEvaluator::LogicalExpression::LogicalExpression(const std::string& operation, Expression* left, Expression* right) : operation(operation), left(left), right(right) {
        }

        ConditionEvaluator::LogicalExpression::~LogicalExpression() {
            if (left) {
                delete left;
            }
            
            if (right) {
                delete right;
            }
        }

        bool ConditionEvaluator::LogicalExpression::Evaluate() {
            if (left == nullptr || right == nullptr) {
                return false;
            }

            if (operation == "&&") {
                return left->Evaluate() && right->Evaluate();
            } else if (operation == "||") {
                return left->Evaluate() || right->Evaluate();
            } else {
                return false;
            }
        }

        ConditionEvaluator::Tokenizer::Tokenizer(const std::string& expression) : str(expression) {
        }

        ConditionEvaluator::FlagExpression* ConditionEvaluator::Tokenizer::Flag() {
            this->SkipWhiteSpace();

            std::string key;
            char ch = this->str.peek();

            while (std::isalnum(ch) || ch == '.') {
                ch = this->str.get();
                key += ch;
                ch = this->str.peek();
            }

            if (key.length() > 0) {
                return new FlagExpression(key);
            }

            return nullptr;
        }

        ConditionEvaluator::NegationExpression* ConditionEvaluator::Tokenizer::Negation() {
            this->SkipWhiteSpace();

            if (this->Character('!')) {
                Expression* operand = this->Flag();

                if (operand) {
                    return new NegationExpression(operand);
                }
            }

            return nullptr;
        }

        bool ConditionEvaluator::Tokenizer::Character(char expected) {
            this->SkipWhiteSpace();

            char ch = this->str.peek();

            if (ch == expected) {
                this->str.get();

                return true;
            }

            return false;
        }

        bool ConditionEvaluator::Tokenizer::Token(const std::string& expected) {
            this->SkipWhiteSpace();

            int mark = this->Mark();

            for (char ch : expected) {
                if (this->str.peek() != ch) {
                    this->Reset(mark);

                    return false;
                }

                this->str.get();
            }

            return true;
        }

        bool ConditionEvaluator::Tokenizer::AtEnd() {
            this->SkipWhiteSpace();

            return this->str.peek() == std::char_traits<char>::eof();
        }

        int ConditionEvaluator::Tokenizer::Mark() {
            return this->str.tellg();
        }

        void ConditionEvaluator::Tokenizer::Reset(int mark) {
            this->str.clear();
            this->str.seekg(mark);
        }

        void ConditionEvaluator::Tokenizer::SkipWhiteSpace() {
            char ch = this->str.peek();

            while (ch == ' ' || ch == '\t') {
                ch = this->str.get();
                ch = this->str.peek();
            }
        }

        ConditionEvaluator::Parser::Parser(const std::string& expression) {
            this->tokens = new Tokenizer(expression);
        }

        ConditionEvaluator::Parser::~Parser() {
            if (tokens) {
                delete tokens;
            }
        }

        ConditionEvaluator::Expression* ConditionEvaluator::Parser::Expression() {
            return this->Or();
        }

        ConditionEvaluator::Expression* ConditionEvaluator::Parser::Or() {
            int mark = this->tokens->Mark();
            ConditionEvaluator::Expression* left = this->And();

            if (left) {
                while (this->tokens->Token("||")) {
                    ConditionEvaluator::Expression* right = this->And();

                    if (right) {
                        left = new LogicalExpression("||", left, right);
                    } else {
                        delete left;
                        left = nullptr;

                        break;
                    }
                }
            }

            if (left == nullptr) {
                this->tokens->Reset(mark);
            }

            return left;
        }

        ConditionEvaluator::Expression* ConditionEvaluator::Parser::And() {
            int mark = this->tokens->Mark();
            ConditionEvaluator::Expression* left = this->Unary();

            if (left) {
                while (this->tokens->Token("&&")) {
                    ConditionEvaluator::Expression* right = this->Unary();

                    if (right) {
                        left = new LogicalExpression("&&", left, right);
                    } else {
                        delete left;
                        left = nullptr;

                        break;
                    }
                }
            }

            if (left == nullptr) {
                this->tokens->Reset(mark);
            }

            return left;
        }

        ConditionEvaluator::Expression* ConditionEvaluator::Parser::Unary() {
            int mark = this->tokens->Mark();

            if (this->tokens->Character('!')) {
                ConditionEvaluator::Expression* operand = this->Unary();

                if (operand) {
                    return new NegationExpression(operand);
                } else {
                    this->tokens->Reset(mark);
                }
            }

            return this->Primary();
        }

        ConditionEvaluator::Expression* ConditionEvaluator::Parser::Primary() {
            int mark = this->tokens->Mark();
            ConditionEvaluator::Expression* flag = this->tokens->Flag();

            if (flag) {
                return flag;
            }

            this->tokens->Reset(mark);

            if (this->tokens->Character('(')) {
                ConditionEvaluator::Expression* exp = this->Expression();

                if (exp && this->tokens->Character(')')) {
                    return exp;
                } else {
                    if (exp) {
                        delete exp;
                    }

                    this->tokens->Reset(mark);
                }
            }

            return nullptr;
        }
    }
}
