#pragma once
#include <stack>
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
    void callFunc(token::TokenDesc* idf, const std::vector<token::TokenDesc*>& exps);
    void startFuncBlock(const std::string& name);
    void endFuncBlock();
}
