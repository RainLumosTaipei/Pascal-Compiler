#include "semantic/Reduce.h"
#include "semantic/Ast.h"
#include "syntax/SyntaxCheck.h"
#include "semantic/VarDef.h"

using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;
using namespace token;


static TokenDesc saveToken(null);

namespace
{
    void getBack()
    {
        auto& waits = syntax::lr::getWaitTokens();
        saveToken = *waits.back();
    }

    TokenDesc* getBackAt(int pos)
    {
        auto& waits = syntax::lr::getWaitTokens();
        auto it = prev(waits.end(), pos);
        return *it;
    }
    
}


namespace
{
    // var_with_type -> id : type
    // var_with_type -> id , var_with_type
    void varDef()
    {
        regisVar(getBackAt(3)->value, saveToken);
    }

    // const_def -> id = const
    // const_def -> const_def ; id = const 
    void constDef()
    {
        regisConstWithValue(getBackAt(3)->value, saveToken);
    }

    void negNum()
    {
        getBack();
        saveToken.value.insert(0, "-");
    }

}




inline ReduceTable& getReduceTable()
{
    static ReduceTable reduceTable{
        
            {22, constDef},
            {23, constDef},

            {24, getBack},
            {25, negNum},
            {26, getBack},
            {27, getBack},

            {32, varDef},
            {33, varDef},
            
            {36, getBack},
            {37, getBack},
            {38, getBack},
            {39, getBack},
        };
    return reduceTable;
}

void semantic::callReduce(size_t id)
{
    auto& table = getReduceTable();
    if(table.find(id) == table.end()) return;
    table[id]();
}
