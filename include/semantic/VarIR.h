#pragma once

#include <llvm/IR/DerivedTypes.h>
#include "FuncIR.h"
#include "llvm/IR/Value.h"
#include "lex/Token.h"
#include "lex/Lexer.h"

namespace semantic
{
    using IntPair = std::pair<int, int>;


    void regisVar(token::TokenDesc*, token::TokenDesc* t, bool isGlobal);
    void regisArg(llvm::Argument& arg);
    void regisConst(const token::TokenDesc*, token::TokenDesc* cont, bool isGlobal);

    void getArrayElement(token::TokenDesc* id, const std::vector<token::TokenDesc*>& pos);
}
