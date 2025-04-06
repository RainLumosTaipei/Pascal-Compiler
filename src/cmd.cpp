#include "cmd.h"

#include <iostream>

#include "semantic/Ast.h"
#include "syntax/SyntaxCheck.h"
#include "syntax/SyntaxEntry.h"
#include "syntax/SyntaxLr.h"


namespace
{
    void getFilename(char** filename, char*** argv)
    {
        if('\0' == (*argv)[0][2])
        {
            *filename = (*argv)[1];
            ++ *argv;
        }
        else
            *filename = &(*argv)[0][2];
    }
}

void cmd(int argc, char** argv)
{
    if (argc < 2) {
        usage();
        exit(1);
    }

    char* filename;
    ++argv;
    while(argv[0] != nullptr)
    {
        if('-' != argv[0][0])
        {
            opMap.emplace(argv[0], obj);
            continue;
        }
        
        switch(argv[0][1])
        {
        case 'g':
            ++debugFlag;
            break;
        case 'W':
            ++warnFlag;
            break;
        case 'O':
            if('\0' == argv[0][2])
                optFlag = 1;
            else
                optFlag = argv[0][2] - '0';
            break;
            
        case 'c':
            getFilename(&filename, &argv);
            opMap.emplace(filename, obj);
            break;
        case 'S':
            getFilename(&filename, &argv);
            opMap.emplace(filename, ll);
            break;
        case 'o':
            getFilename(&filename, &argv);
            outFilename = filename;
            break;
        
        default:
            break;
        }

        ++argv;
    }

    cmpl();
}

void usage()
{
    std::cout << "Usage: " << '\n'
    << "\t-O0\t" << "no optimize" << '\n'
    << "\t-O\t" << "use optimize" << '\n'
    << "\t-g\t" << "generate debug info" << '\n'
    << "\t-W\t" << "use warning hint" << '\n'
    
    << "\t-c\t" << "<filename> compile to .o file" << '\n'
    << "\t-S\t" << "<filename> compile to .ll file" << '\n'
    << "\t-o\t" << "<filename> rename output filename" << '\n'
    << std::endl;
}

void cmpl()
{
    syntax::initSyntaxes();
    
    if(syntax::lr::loadTable())
    {
        return;
    }
    ast::initPass();
    
    for(auto& kv : opMap)
    {
        if(token::lex(kv.first))
        {
            std::cout << "[0/4] lex wrong in " << kv.first << std::endl;
            continue;
        }
        if(syntax::lr::lrCheck())
        {
            std::cout << "[1/4] syntax check wrong in " << kv.first << std::endl;
            continue;
        }
        if(ast::saveIR(kv.first))
        {
            std::cout << "[2/4] generate IR wrong in " << kv.first << std::endl;
            continue;
        }
        if(kv.second == obj && ast::saveASM(kv.first))
        {
            std::cout << "[3/4] generate asm wrong in " << kv.first << std::endl;
        }
    }
}