#include "parser.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            Parser::Parser(const std::string& expression) {
                this->tokens = new Tokenizer(expression);
            }

            Parser::~Parser() {
                if (tokens) {
                    delete tokens;
                }
            }

            Expressions::Expression* Parser::Expression() {
                return this->Or();
            }

            Expressions::Expression* Parser::Or() {
                int mark = this->tokens->Mark();
                Expressions::Expression* left = this->And();

                if (left) {
                    while (this->tokens->Token("||")) {
                        Expressions::Expression* right = this->And();

                        if (right) {
                            left = new Expressions::LogicalExpression("||", left, right);
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

            Expressions::Expression* Parser::And() {
                int mark = this->tokens->Mark();
                Expressions::Expression* left = this->Unary();

                if (left) {
                    while (this->tokens->Token("&&")) {
                        Expressions::Expression* right = this->Unary();

                        if (right) {
                            left = new Expressions::LogicalExpression("&&", left, right);
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

            Expressions::Expression* Parser::Unary() {
                int mark = this->tokens->Mark();

                if (this->tokens->Character('!')) {
                    Expressions::Expression* operand = this->Unary();

                    if (operand) {
                        return new Expressions::NegationExpression(operand);
                    } else {
                        this->tokens->Reset(mark);
                    }
                }

                return this->Primary();
            }

            Expressions::Expression* Parser::Primary() {
                int mark = this->tokens->Mark();
                Expressions::Expression* flag = this->tokens->Flag();

                if (flag) {
                    return flag;
                }

                this->tokens->Reset(mark);

                if (this->tokens->Character('(')) {
                    Expressions::Expression* exp = this->Expression();

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
}
