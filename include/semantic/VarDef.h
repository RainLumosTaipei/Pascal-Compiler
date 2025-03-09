#pragma once
#include <map>
#include <llvm/IR/Value.h>

#include "lex/Token.h"


namespace semantic
{
    std::map<std::string, llvm::Value*>& getIdTable();
    void regisVar(std::string id, token::TokenState type);
}
