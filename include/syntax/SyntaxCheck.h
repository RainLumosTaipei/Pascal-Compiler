#pragma once
#include "lex/Lexer.h"

namespace syntax::lr
{
    int lrCheck();
    std::deque<token::TokenDesc*>& getWaitTokens();
}
