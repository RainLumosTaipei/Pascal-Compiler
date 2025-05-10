#pragma once
#include "lex/Lexer.h"

namespace syntax::lr
{
    int syntaxCheck();
    std::deque<token::TokenDesc*>& getWaitTokens();
}
