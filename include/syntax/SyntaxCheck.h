#pragma once
#include "lex/Lexer.h"
#include "SyntaxEntry.h"

namespace syntax::lr
{
    void lrCheck();
    std::deque<token::TokenDesc*>& getWaitTokens();
}
