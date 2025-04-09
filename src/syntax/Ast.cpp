//
// Created by semiregular on 2025-04-09.
//

#include "syntax/Ast.h"

using namespace std;
using namespace syntax;
using namespace token;

static Ast* head;

AstStack& syntax::getAstStack(){
    static AstStack astStack;
    return astStack;
}

void syntax::reduceAst(token::TokenDesc *t, size_t n) {
    Ast* newAst = new Ast(t);
    for(; n > 0; n--){
        newAst->child.push_back(getAstStack().top());
        getAstStack().pop();
    }
    getAstStack().push(newAst);
}

void syntax::setAstHead() {
    head = getAstStack().top();
    getAstStack().pop();
}
