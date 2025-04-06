#include "cmd.h"
#include "semantic/Ast.h"

#include "syntax/SyntaxEntry.h"
#include "syntax/SyntaxLl.h"
#include "syntax/SyntaxLr.h"

using namespace std;

// pcc -O -c open_set/00_main.pas
int main(int argc, char* argv[])
{
    cmd(argc, argv);
    return 0;
}

void init()
{
    syntax::initSyntaxes();
    syntax::ll::initFirst();
    syntax::lr::initLr();
    syntax::lr::saveTable();
    syntax::ll::printFirst();
    syntax::ll::printFollow();
}



