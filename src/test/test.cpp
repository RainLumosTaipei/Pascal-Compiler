#include "test/test.h"

#include <gtest/gtest.h>

#include "semantic/Ast.h"
#include "syntax/SyntaxCheck.h"
#include "syntax/SyntaxLr.h"

using namespace std;

namespace
{
    bool testOne(string& filename)
    {
        ast::getModules().clear();
        ast::resetModule();
        return 
            !(token::lex(filename) ||
            syntax::lr::syntaxCheck() ||
            ast::saveIR(filename) ||
            ast::saveASM(filename));
    }

    bool testLinkOne(string& filename)
    {
        ast::resetModule();
        return 
            !(token::lex(filename) ||
            syntax::lr::syntaxCheck() ||
            ast::saveIR(filename) ||
            ast::saveASM(filename));
    }

    bool testLink(string filename)
    {
        return 
            !( ast::link() ||
            ast::saveIR(filename) ||
            ast::saveASM(filename));
    }
    
    vector<string> simpleVarFile {
        "misc/test_set/var/var_def.pas",
        "misc/test_set/var/var_lval.pas",
        "misc/test_set/var/var_rval.pas",
    };

    vector<string> constVarFile {
        "misc/test_set/var/const_rval.pas",
        "misc/test_set/var/const_def.pas",
    };

    vector<string> arrayVarFile {
        "misc/test_set/var/array_def.pas",
        "misc/test_set/var/array_lval.pas",
        "misc/test_set/var/array_rval.pas",
    };
    
    vector<string> funcFile {
        "misc/test_set/func/func_def.pas",
        "misc/test_set/func/func_call.pas",
    };

    vector<string> procFile {
        "misc/test_set/func/proc_call.pas",
        "misc/test_set/func/proc_def.pas",
    };

    vector<string> linkFile {
        "misc/test_set/link/main.pas", 
        "misc/test_set/link/add.pas", 
    };

    vector<string> opStmtFile {
        "misc/test_set/stmt/unary_op.pas", 
        "misc/test_set/stmt/binary_op.pas", 
    };

    vector<string> ioStmtFile {
        "misc/test_set/stmt/read.pas", 
        "misc/test_set/stmt/write.pas", 
    };

    vector<string> loopStmtFile {
        "misc/test_set/stmt/if.pas", 
        "misc/test_set/stmt/for.pas", 
    };

    vector<string> errFile {
        
        "misc/test_set/err/scope.pas", 
    };

    vector<string> warnFile {
        "misc/test_set/warn/type.pas",
        "misc/test_set/warn/const.pas", 
    };
}

void test::testAll()
{
    syntax::initSyntaxes();
    if(syntax::lr::loadTable()) return;
    ast::initPass();
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
}


TEST(var, simple)
{
    for (auto& file : simpleVarFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(var, const)
{
    for (auto& file : constVarFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(var, array)
{
    for (auto& file : arrayVarFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(stmt, operator)
{
    for (auto& file : opStmtFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(stmt, io)
{
    for (auto& file : ioStmtFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(stmt, loop)
{
    for (auto& file : loopStmtFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(block, function)
{
    for (auto& file : funcFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(block, procedure)
{
    for (auto& file : procFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(warn, def)
{
    for (auto& file : warnFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(err, def)
{
    for (auto& file : errFile)
        EXPECT_EQ(false, testOne(file));
}

TEST(link, def)
{
    ast::getModules().clear();
    for (auto& file : linkFile)
        EXPECT_EQ(true, testLinkOne(file));
    EXPECT_EQ(true, testLink("misc/test_set/link/output.pas"));
}



