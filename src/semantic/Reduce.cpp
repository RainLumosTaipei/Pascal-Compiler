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
static ArrayDesc arrayDesc;
static bool isArrayDef = false;

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

    int getSize(int pos)
    {
        int r = stoi(getBackAt(pos)->value);
        int l = stoi(getBackAt(pos + 2)->value);
        return r - l + 1;
    }
    
}

namespace
{
    // var_with_type -> id : type
    // var_with_type -> id , var_with_type
    void varDef()
    {
        if (!isArrayDef)
            return regisVar(getBackAt(3)->value, saveToken);
        regisVarArray(getBackAt(3)->value, arrayDesc);
        isArrayDef = false;
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

    // period_with_type -> digit .. digit ] key_of type_base
    void arrayTypeBase()
    {
        getArrayType(saveToken, arrayDesc, getSize(4));
    }

    // period_with_type -> digit .. digit , period_with_type
    void arrayType()
    {
        getArrayType(arrayDesc, getSize(3));
    }

    // type -> key_array [ period_with_type
    void changeArrayDef()
    {
        isArrayDef = true;
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
            {35, changeArrayDef},
            
            {36, getBack},
            {37, getBack},
            {38, getBack},
            {39, getBack},

            {40, arrayType},
            {41, arrayTypeBase},
        };
    return reduceTable;
}

void semantic::callReduce(size_t id)
{
    auto& table = getReduceTable();
    if(table.find(id) == table.end()) return;
    table[id]();
}
