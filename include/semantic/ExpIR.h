#pragma once

#include "llvm/IR/Value.h"

#include "lex/Lexer.h"
#include "lex/Token.h"

namespace semantic
{
    void load(llvm::Value* id);
    void store(token::TokenDesc* exp, token::TokenDesc* ptr);
    void loadIfPointer(SymbolEntry& entry);
    void num(token::TokenDesc* t);
    void boolean(token::TokenDesc* t);
    void unaryOp(token::TokenDesc* op, token::TokenDesc* factor);
    void binaryOp(token::TokenDesc* op, token::TokenDesc* L, token::TokenDesc* R, token::TokenDesc* ret);
}
