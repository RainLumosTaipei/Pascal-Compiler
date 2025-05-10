#pragma once
#include <iostream>
#include <string>

#include "lex/Lexer.h"

namespace syntax
{
    class SyntaxErr : public std::exception
    {
    public:
        std::string msg;
        size_t line;
        size_t col;
        std::string name;

        SyntaxErr(std::string msg, const token::TokenDesc* desc);
    };

    inline std::ostream& operator << (std::ostream& os, const SyntaxErr& err)
    {
        std::cout << "Syntax exception at " << token::Lexer::filename
            << ", line " << err.line
            << ", col " << err.col
            << " [" << err.name
            << "] " << err.msg;
        return os;
    }
}

