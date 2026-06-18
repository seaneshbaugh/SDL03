#include "condition.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            Condition::Condition(const std::string& condition) : condition(condition) {
                this->parser = new Parser(condition);
                this->expression = this->parser->Expression();
            }

            Condition::~Condition() {
                if (this->parser) {
                    delete this->parser;
                }

                if (this->expression) {
                    delete this->expression;
                }
            }

            bool Condition::Evaluate() {
                if (this->expression) {
                    return this->expression->Evaluate();
                } else {
                    return false;
                }
            }
        }
    }
}
