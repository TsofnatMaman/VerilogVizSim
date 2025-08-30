#pragma once
#include "lexer.h"

namespace mvs
{
    class Parser
    {
    public:
        explicit Parser(const std::vector<Token> &) {}
        bool parseModuleStub() const { return true; }
    };
}