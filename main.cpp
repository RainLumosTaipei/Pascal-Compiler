#include "Lexer.h"
#include "SyntaxEntry.h"
#include "SyntaxLl.h"
#include "SyntaxLr.h"

using namespace std;

int main() {


    syntax::initSyntaxes();

    if (0){
        syntax::ll::initFirst();
        syntax::lr::initLr();
        syntax::lr::saveTable();
        syntax::ll::printFirst();
        syntax::ll::printFollow();
    }
    else{
        syntax::lr::loadTable();
        token::lex("open_set/02_var_defn3.pas");
        //token::printTokens();
        syntax::lr::lrCheck();
    }

    return 0;
}
