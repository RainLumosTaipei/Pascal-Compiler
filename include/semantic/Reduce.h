#pragma once

#include <unordered_map>


#include "lex/Lexer.h"

namespace semantic
{
    using ReduceFn = void(*)(token::TokenDesc*);


    using ReduceTable = std::unordered_map<size_t, ReduceFn>;

    token::TokenDesc* callReduce(size_t id, token::TokenState t);
}
