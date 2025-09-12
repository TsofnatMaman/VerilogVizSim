#include "mvs/Lexer.h"
#include "mvs/utils.h"

using namespace mvs;

char Lexer::get()
{
    if (eof())
        return '\0';
    char c = src_[i_++];
    if (c == '\n')
    {
        line_++;
        col_ = 1;
    }
    else
    {
        col_++;
    }
    return c;
}

void Lexer::skip_space_and_comments()
{
    while (!eof())
    {
        char c = peek();
        if (std::isspace(static_cast<unsigned char>(c)))
        {
            get();
        }
        else if (c == '/' && i_ + 1 < src_.size() && src_[i_ + 1] == '/')
        {
            while (!eof() && get() != '\n');
        }
        else if (c == '/' && i_ + 1 < src_.size() && src_[i_ + 1] == '*')
        {
            get(); get();
            while (!eof()) {
                if (peek() == '*' && i_ + 1 < src_.size() && src_[i_ + 1] == '/') {
                    get(); get(); 
                    break;
                }
                get();
            }
        }
        else break;
    }
}

Token Lexer::lex_identifier_or_keyword()
{
    int start_line = line_;
    int start_col  = col_;

    std::string text;

    while(is_identifier_char(peek())){
        text.push_back(get());
    }

    TokenKind kind = is_keyword(text)? TokenKind::KEYWORD : TokenKind::IDENTIFIER;

    return Token{kind, text, start_line, start_col};
}

Token Lexer::lex_number(){
    int start_line = line_;
    int start_col = col_;

    //TODO: complete
    return Token{};
}
