#include "Lexer.h"
#include "SyntaxEntry.h"
#include "SyntaxLl.h"
#include "SyntaxLr.h"

using namespace std;

static void init(){
    syntax::ll::initFirst();
    syntax::lr::initLr();
    syntax::lr::saveTable();
    syntax::ll::printFirst();
    syntax::ll::printFollow();
}

static void test(){
    syntax::lr::loadTable();
    token::lex("open_set/69_matrix_tran.pas");
    //token::printTokens();
    syntax::lr::lrCheck();
}

int main() {

    syntax::initSyntaxes();

    //init();
    test();

    return 0;
}
