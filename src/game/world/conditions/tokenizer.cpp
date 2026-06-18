#include "tokenizer.hpp"

namespace Game {
    namespace World {
        namespace Conditions {
            Tokenizer::Tokenizer(const std::string& expression) : str(expression) {
            }

            Expressions::FlagExpression* Tokenizer::Flag() {
                this->SkipWhiteSpace();

                std::string key;
                char ch = this->str.peek();

                while (std::isalnum(ch) || ch == '.') {
                    ch = this->str.get();
                    key += ch;
                    ch = this->str.peek();
                }

                if (key.length() > 0) {
                    return new Expressions::FlagExpression(key);
                }

                return nullptr;
            }

            Expressions::NegationExpression* Tokenizer::Negation() {
                this->SkipWhiteSpace();

                if (this->Character('!')) {
                    Expressions::FlagExpression* operand = this->Flag();

                    if (operand) {
                        return new Expressions::NegationExpression(operand);
                    }
                }

                return nullptr;
            }

            bool Tokenizer::Character(char expected) {
                this->SkipWhiteSpace();

                char ch = this->str.peek();

                if (ch == expected) {
                    this->str.get();

                    return true;
                }

                return false;
            }

            bool Tokenizer::Token(const std::string& expected) {
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

            bool Tokenizer::AtEnd() {
                this->SkipWhiteSpace();

                return this->str.peek() == std::char_traits<char>::eof();
            }

            int Tokenizer::Mark() {
                return this->str.tellg();
            }

            void Tokenizer::Reset(int mark) {
                this->str.clear();
                this->str.seekg(mark);
            }

            void Tokenizer::SkipWhiteSpace() {
                char ch = this->str.peek();

                while (ch == ' ' || ch == '\t') {
                    ch = this->str.get();
                    ch = this->str.peek();
                }
            }
        }
    }
}
