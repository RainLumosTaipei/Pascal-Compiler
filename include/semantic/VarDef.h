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
    typedef std::vector<IntPair> ArrayShape;
    typedef struct
    {
        llvm::ArrayType* arrayType;
        llvm::Constant* constant;
    } ArrayDesc;
    
    
    void regisVar(const token::TokenDesc* , token::TokenState t, bool isGlobal);
    void regisVar(const Para& para);
    void regisConstWithValue(const token::TokenDesc* , token::TokenDesc* t, bool isGlobal);
    void regisVarArray(const token::TokenDesc* , const ArrayDesc& desc, bool isGlobal);
    
    void getArrayType(ArrayDesc& desc, int size);
    void getArrayType(token::TokenDesc* t, ArrayDesc& desc, int size);
}
