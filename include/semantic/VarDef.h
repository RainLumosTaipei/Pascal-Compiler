#pragma once
#include <map>
#include <llvm/IR/DerivedTypes.h>

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
    
    std::map<std::string, llvm::Value*>& getIdTable();
    void regisVar(std::string id, token::TokenDesc& t);
    void regisConstWithValue(const std::string& id, token::TokenDesc& t);
    void regisVarArray(const std::string& id, const ArrayDesc& desc);
    void getArrayType(ArrayDesc& desc, int size);
    void getArrayType(token::TokenDesc& t, ArrayDesc& desc, int size);
}
