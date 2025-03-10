#include "semantic/Reduce.h"
#include "semantic/Ast.h"
#include "syntax/SyntaxCheck.h"
#include "semantic/VarDef.h"
#include "semantic/FuncDef.h"

using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;
using namespace token;

namespace
{
    // typebase
    TokenDesc saveToken(null);
    // array
    ArrayDesc arrayDesc;
    bool isArrayDef = false;
    // func para
    FuncDesc funcDesc;
}



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

namespace
{
    // id_with_type -> id : type_base
    // id_with_type -> id , id_with_type
    void addPara()
    {
        funcDesc.paras.emplace_back(saveToken.token, getBackAt(3)->value);
    }

    // sub_prog_head -> key_func idf formal_para : type_base
    void funcDef()
    {
        funcDesc.rev = saveToken.token;
        funcDesc.name = getBackAt(4)->value;
        regisFunc(funcDesc);
        funcDesc.paras.clear();
    }

    // sub_prog_head -> key_proc idf formal_para
    void procDef()
    {
        funcDesc.name = getBackAt(2)->value;
        funcDesc.rev = null;
        regisFunc(funcDesc);
        funcDesc.paras.clear();
    }
}



inline ReduceTable& getReduceTable()
{
    static ReduceTable reduceTable{

            {9, procDef},
            {10, funcDef},
        
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
            {42, addPara},
            {43, addPara}
        };
    return reduceTable;
}

void semantic::callReduce(size_t id)
{
    auto& table = getReduceTable();
    if(table.find(id) == table.end()) return;
    table[id]();
}
