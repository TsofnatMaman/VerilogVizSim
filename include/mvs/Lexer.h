#pragma once
#include <string>
#include <vector>

namespace mvs
{
    enum class TokenKind
    {
        END
    };

    struct Token
    {
        TokenKind type;
        std::string text;
    };

    class Lexer
    {

    public:
        explicit Lexer(std::string src) : src_(std::move(src)) {}
        std::vector<Token> Tokenize() const { return {Token{TokenKind::END, ""}}; }

    private:
        std::string src_;
    };
}