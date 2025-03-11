#pragma once
#include "SymbolTable.h"
#include "llvm/IR/Value.h"

#include "lex/Lexer.h"
#include "lex/Token.h"

namespace semantic
{
    void load(llvm::Value* id);
    void store(llvm::Value* value, llvm::Value* id);
    void loadIfPointer(SymbolEntry& entry);
    void num(const token::TokenDesc* t, SymbolEntry& entry);
    void unaryOp(token::TokenState op, SymbolEntry& );
    void binaryOp (token::TokenState op, SymbolEntry& L, SymbolEntry& R, SymbolEntry& entry);
}
