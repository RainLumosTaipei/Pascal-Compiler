#pragma once

#include <unordered_map>

#include "SymbolTable.h"
#include "lex/Lexer.h"

namespace semantic{

    typedef void (*ReduceFn)(token::TokenDesc*);
    

    typedef std::unordered_map<size_t, ReduceFn> ReduceTable;

    token::TokenDesc* callReduce(size_t id, token::TokenState t);
}
