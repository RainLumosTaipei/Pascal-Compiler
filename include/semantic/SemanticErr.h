#pragma once
#include <iostream>
#include <string>

#include "lex/Lexer.h"
#include "semantic/Ast.h"

namespace semantic
{
    class SemanticErr : public std::exception
    {
    public:
        std::string msg;
        size_t line;
        size_t col;
        std::string name;

        SemanticErr(std::string msg, const token::TokenDesc* desc);
    };

    inline std::ostream& operator <<(std::ostream& os, const SemanticErr& err)
    {
        std::cout << "Semantic exception at " << ast::getModule().getSourceFileName()
            << ", line " << err.line
            << ", col " << err.col
            << " [" << err.name
            << "] " << err.msg;
        return os;
    }
}
