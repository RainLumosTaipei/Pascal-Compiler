#pragma once

#include "lex/Lexer.h"
#include <string>

namespace token
{
    class LexErr : public std::exception
    {
    public:
        std::string msg;
        size_t line;
        size_t col;
        std::string name;

        LexErr(std::string msg, const std::string& name);
    };

    inline std::ostream& operator <<(std::ostream& os, const LexErr& err)
    {
        std::cout << "Lexical exception at " << Lexer::filename
            << ", line " << err.line
            << ", col " << err.col
            << " [" << err.name
            << "] " << err.msg;
        return os;
    }
}
