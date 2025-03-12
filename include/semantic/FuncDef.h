#pragma once
#include <vector>
#include <llvm/IR/Function.h>


#include "lex/Lexer.h"
#include "lex/Token.h"

namespace semantic
{
    typedef struct
    {
        std::vector<token::TokenDesc*> paraType;
        std::vector<token::TokenDesc*> paraName;
        token::TokenDesc* rev;
        token::TokenDesc* name;
        bool isVoid;
    } FuncDesc;

    void regisFunc(const FuncDesc& desc);
    void retFunc(token::TokenDesc* ret);
    void retFunc();
    llvm::Function* getFunc(const std::string& name);
}
