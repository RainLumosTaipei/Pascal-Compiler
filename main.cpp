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

static void test(const string& fileName)
{
    syntax::lr::loadTable();
    token::lex(fileName);
    //token::printTokens();
    syntax::lr::lrCheck();
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // 获取传入的文件名
    syntax::initSyntaxes();

    //init();
    test(argv[1]);

    return 0;
}


