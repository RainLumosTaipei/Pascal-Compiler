#pragma once
#include <map>
#include "llvm/IR/Value.h"

#include "lex/Token.h"
#include "lex/Lexer.h"


namespace semantic
{
    typedef std::pair<int, int> IntPair;
    typedef std::vector<IntPair> ArrayShape;
    
    std::map<std::string, llvm::Value*>& getIdTable();
    void regisVar(std::string id, token::TokenDesc& t);
    void regisConstWithValue(const std::string& id, token::TokenDesc& t);
    void regisArray(const std::string& id, token::TokenDesc& t, ArrayShape& shape);
}
