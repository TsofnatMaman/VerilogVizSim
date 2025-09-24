#pragma once
#include "lexer.h"
#include "ast.h"
#include <vector>
#include <stdexcept>

namespace mvs
{

    class Parser
    {
    private:
        const std::vector<Token> &tokens_;
        size_t pos_ = 0;

    public:
        explicit Parser(const std::vector<Token> &tokens) : tokens_(tokens) {}

        const Token &current() const
        {
            if (pos_ < tokens_.size())
                return tokens_[pos_];
            throw std::out_of_range("Parser: past end of tokens");
        }

        void advance()
        {
            if (pos_ < tokens_.size())
                pos_++;
        }

        void expect(TokenKind kind)
        {
            if (current().type != kind)
            {
                throw std::runtime_error("Unexpected token: " + current().text);
            }
            advance();
        }
    };

} // namespace mvs
