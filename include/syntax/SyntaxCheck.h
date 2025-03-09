#pragma once
#include "lex/Lexer.h"

namespace syntax::lr
{
    void lrCheck();
    std::deque<token::TokenDesc*>& getWaitTokens();
}
