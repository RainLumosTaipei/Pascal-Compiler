#pragma once
#include <map>
#include <llvm/IR/DerivedTypes.h>

#include "FuncDef.h"
#include "llvm/IR/Value.h"

#include "lex/Token.h"
#include "lex/Lexer.h"


namespace semantic
{
    typedef std::pair<int, int> IntPair;
    
    void regisVar(token::TokenDesc* , token::TokenDesc* t, bool isGlobal);
    void regisConstWithValue(const token::TokenDesc* , token::TokenDesc* cont, bool isGlobal);
    
    void typeInt(token::TokenDesc* type);
    void typeChar(token::TokenDesc* type);
    void typeBool(token::TokenDesc* type);
    void typeReal(token::TokenDesc* type);
    void typeArray(token::TokenDesc* type, int size);
}
