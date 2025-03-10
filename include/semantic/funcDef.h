#pragma once
#include <vector>

#include "lex/Lexer.h"
#include "lex/Token.h"

namespace semantic
{
    typedef std::pair<token::TokenState, std::string> Para;
    typedef std::vector<Para> ParaList;
    typedef struct
    {
        ParaList paras;
        token::TokenState rev;
        std::string name;
    }FuncDesc;

    void regisFunc(const FuncDesc& desc);
}
