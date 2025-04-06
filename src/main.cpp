#include "u.h"
#include "semantic/Ast.h"
#include "syntax/SyntaxCheck.h"
#include "syntax/SyntaxEntry.h"
#include "syntax/SyntaxLl.h"
#include "syntax/SyntaxLr.h"

using namespace std;



int main(int argc, char* argv[])
{
    if (argc < 2) {
        usage();
        return 1;
    }

    for(int i = 1; i < argc; ++i)
    {
        if(i == argc - 1)
        {
            test(argv[i]);
        }
    }
    
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

void test(const string& fileName)
{
    syntax::initSyntaxes();
    syntax::lr::loadTable();
    token::lex(fileName);
    //token::printTokens();
    ast::initPass();
    syntax::lr::lrCheck();
    ast::saveIR();
    ast::saveASM();
}


void usage()
{
    cout << "Usage: " << '\n'
    << "\t-O0\t" << "no optimize" << '\n'
    << "\t-O\t" << "use optimize" << '\n'
    << "\t-g\t" << "generate debug info" << '\n'
    << "\t-Wall\t" << "use all warning" << '\n'
    
    << "\t-c\t" << "<filename> compile to .o file" << '\n'
    << "\t-S\t" << "<filename> compile to .ll file" << '\n'
    << "\t-o\t" << "<filename> rename output filename" << '\n'
    << '\n';
}