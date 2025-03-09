#include "lex/Lexer.h"
#include "syntax/SyntaxLl.h"
#include "syntax/SyntaxLr.h"
#include "syntax/SyntaxCheck.h"

using namespace std;

static void init()
{
    syntax::ll::initFirst();
    syntax::lr::initLr();
    syntax::lr::saveTable();
    syntax::ll::printFirst();
    syntax::ll::printFollow();
}

static void test()
{
    syntax::lr::loadTable();
    token::lex("open_set/03_arr_defn2.pas");
    syntax::lr::lrCheck();
}

int main() {

    syntax::initSyntaxes();

    //init();
    test();

    return 0;
}
