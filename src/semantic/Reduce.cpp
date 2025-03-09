#include "semantic/Reduce.h"
#include "semantic/Ast.h"
#include "syntax/SyntaxCheck.h"
#include "semantic/VarDef.h"

using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;


static token::TokenState  varType;



namespace
{
    // var_with_type -> id : type
    // var_with_type -> id , var_with_type
    void varDef()
    {
        auto& waits = syntax::lr::getWaitTokens();
        auto it = prev(waits.end(), 3);
        regisVar((*it)->value, varType);
    }
}


namespace
{
    // type_base -> integer
    void typeInt()
    {
        varType = token::TokenState::type_int;
    }

    // type_base -> char
    void typeChar()
    {
        varType = token::TokenState::type_char;
    }

    // type_base -> real
    void typeReal()
    {
        varType = token::TokenState::type_real;
    }

    // type_base -> bool
    void typeBool()
    {
        varType = token::TokenState::type_bool;
    }
    
}

inline ReduceTable& getReduceTable()
{
    static ReduceTable reduceTable{
        
        {32, varDef},
        {33, varDef},
        
        {36, typeInt},
        {37, typeReal},
        {38, typeChar},
        {39, typeBool},
        };
    return reduceTable;
}


void semantic::callReduce(size_t id)
{
    auto& table = getReduceTable();
    if(table.find(id) == table.end()) return;
    table[id]();
}
