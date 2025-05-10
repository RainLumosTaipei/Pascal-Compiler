#include "cmd.h"
#include "semantic/Ast.h"

#include "syntax/SyntaxEntry.h"
#include "syntax/SyntaxLl.h"
#include "syntax/SyntaxLr.h"
#include "gtest/gtest.h"
#include "test/test.h"

using namespace std;

// .\pcc.exe -O -c open_set/00_main.pas
// .\pcc.exe -g -c misc/test_set/link/add.pas  misc/test_set/link/main.pas
// .\pcc.exe -i
// .\pcc.exe -t
int main(int argc, char* argv[])
{
    cmd(argc, argv);
    return 0;
}
