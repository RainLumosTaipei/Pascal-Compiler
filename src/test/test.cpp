#include "test/test.h"

#include <gtest/gtest.h>

#include "semantic/Ast.h"
#include "syntax/SyntaxCheck.h"
#include "syntax/SyntaxLr.h"

using namespace std;

namespace
{
    bool testOne(string filename)
    {
        ast::resetModule();
        return 
            !(token::lex(filename) ||
            syntax::lr::lrCheck() ||
            ast::saveIR(filename) ||
            ast::saveASM(filename));
    }
}

static vector<string> varFile {
    "misc/test/var/def.pas"
};

static vector<string> funcFile {
    "misc/test/func/def.pas"
};

static vector<string> linkFile {
    "misc/test/var/def.pas"
};


void test::testAll()
{
    syntax::initSyntaxes();
    if(syntax::lr::loadTable()) return;
    ast::initPass();
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
}


TEST(var, def)
{
    for (auto& file : varFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(func, def)
{
    for (auto& file : funcFile)
        EXPECT_EQ(true, testOne(file));
}

TEST(link, def)
{
    for (auto& file : linkFile)
        EXPECT_EQ(true, testOne(file));
}




