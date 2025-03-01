#include "Lexer.h"
#include "SyntaxEntry.h"
#include "SyntaxLl.h"
#include "SyntaxLr.h"

using namespace std;

int main() {

    token::lex("open_set/00_main.pas");
    //token::lex();
    //token::printTokens();

    syntax::initSyntaxes();
    syntax::ll::initFirst();
    //syntax::ll::printFirst();
    //syntax::ll::printFollow();

    //syntax::lr::initLr();
    //syntax::lr::saveTable();
    syntax::lr::loadTable();

    syntax::lr::lrCheck();

    return 0;
}
