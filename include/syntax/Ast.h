//
// Created by semiregular on 2025-04-09.
//

#pragma once

#include <vector>
#include <stack>
#include "lex/Lexer.h"

namespace syntax{
    struct Ast {
        token::TokenDesc* t;
        std::vector<Ast*> child;

        Ast(token::TokenDesc* td) : t(td) {}
    };

    typedef std::stack<Ast*> AstStack;
    AstStack& getAstStack();
    void reduceAst(token::TokenDesc* t, size_t n);
    void setAstHead();

}

