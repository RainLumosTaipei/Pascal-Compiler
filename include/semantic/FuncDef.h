#pragma once
#include <vector>
#include <llvm/IR/Function.h>


#include "lex/Lexer.h"
#include "lex/Token.h"

namespace semantic
{
    typedef std::pair<token::TokenState, token::TokenDesc*> Para;
    typedef std::vector<Para> ParaList;
    typedef struct
    {
        ParaList paras;
        token::TokenState rev;
        token::TokenDesc* name;
    }FuncDesc;

    void regisFunc(const FuncDesc& desc);
    void retFunc(llvm::Value* ret);
    void retFunc();
    llvm::Function* getFunc(const std::string& name);
}
