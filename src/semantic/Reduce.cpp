﻿#include "semantic/Reduce.h"

#include <stack>

#include "semantic/Ast.h"
#include "semantic/Exp.h"
#include "syntax/SyntaxCheck.h"
#include "semantic/VarDef.h"
#include "semantic/FuncDef.h"
#include "semantic/SymbolTable.h"

using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;
using namespace token;

namespace
{
    // typebase
    TokenDesc* saveToken;
    
    // array
    ArrayDesc arrayDesc;
    bool isArrayDef = false;
    
    // func para
    FuncDesc funcDesc;
    bool isGlobal = true;
    bool isVoid = false;

    // exp
    SymbolEntry entry;
    stack<SymbolEntry> expStack;
    TokenState op;
    
}



namespace
{
    void getBack()
    {
        auto& waits = syntax::lr::getWaitTokens();
        saveToken = waits.back();
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
            return regisVar(getBackAt(3), saveToken->token, isGlobal);
        regisVarArray(getBackAt(3), arrayDesc, isGlobal);
        isArrayDef = false;
    }

    // const_def -> id = const
    // const_def -> const_def ; id = const 
    void constDef()
    {
        regisConstWithValue(getBackAt(3), saveToken, isGlobal);
    }

    void negNum()
    {
        getBack();
        saveToken->value.insert(0, "-");
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
        funcDesc.paras.emplace_back(saveToken->token, getBackAt(3));
    }

    // sub_prog_head -> key_func idf formal_para : type_base
    void funcDef()
    {
        funcDesc.rev = saveToken->token;
        funcDesc.name = getBackAt(4);
        regisFunc(funcDesc);
        funcDesc.paras.clear();
        isGlobal = false;
    }

    // sub_prog_head -> key_proc idf formal_para
    void procDef()
    {
        funcDesc.name = getBackAt(2);
        funcDesc.rev = null;
        regisFunc(funcDesc);
        funcDesc.paras.clear();
        isGlobal = false;
        isVoid = true;
    }

    // main -> begin stmt_list end
    void endBlock()
    {
        getSymbolTable().leaveScope();
        if (getSymbolTable().deep() == 1)
            isGlobal = true;
        if(isVoid)
        {
            retFunc();
            isVoid = false;
        }
    }

    
}

namespace
{
    // factor -> var
    void varExp()
    {
        auto* t = getBackAt(1);
        if(getSymbolTable().findVar(t->value, entry))
        {
            loadIfPointer(entry);
            return expStack.push(entry); 
        }
        throw runtime_error("var does not exist");
    }

    // factor -> num
    void numExp()
    {
        auto* t = getBackAt(1);
        semantic::num(t, entry);
        expStack.push(entry);
    }

    // stmt_base -> var := exp 
    void assignExp()
    {
        Value* newValue = expStack.top().val;
        auto* var = getBackAt(3);
        if(getSymbolTable().findVar(var->value, entry))
            return store(newValue, entry.val);
        expStack.pop();
        throw runtime_error("var does not exist");
    }

    // stmt_base -> idf := exp 
    void retExp()
    {
        Value* ret = expStack.top().val;
        getBuilder().CreateRet(ret);
        expStack.pop();
    }

    // factor -> not factor
    // factor -> op_neg factor
    // factor -> op_pos factor
    void unaryExp()
    {
        auto* op = getBackAt(2);
        unaryOp(op->token, expStack.top());
    }

    // exp -> exp op_cmp sub_exp
    // sub_exp -> sub_exp op_add_sub term
    // term -> term op_div_mul factor
    void binaryExp()
    {
        auto r = expStack.top();
        expStack.pop();
        auto l = expStack.top();
        expStack.pop();
        binaryOp(op, l, r, entry);
        expStack.push(entry);
    }

    // op_cmp -> >
    // op_cmp -> < 
    // op_cmp -> =
    // op_cmp -> >=
    // op_cmp -> <=
    // op_cmp -> <>
    // op_add_sub -> +
    // op_add_sub -> -
    // op_add_sub -> or
    // op_div_mul -> div
    // op_div_mul -> *
    // op_div_mul -> mod
    // op_div_mul -> and
    void opExp()
    {
        op = getBackAt(1)->token.token;
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
            {43, addPara},


            {48, opExp},
            {49, opExp},
            {50, opExp},
            {51, opExp},
            {52, opExp},
            {53, opExp},
            {54, opExp},
            {55, opExp},
            {56, opExp},
            {57, opExp},
            {58, opExp},
            {59, opExp},
            {60, opExp},

            {61, endBlock},
        
            {69, assignExp},
            {70, retExp},

            {84, binaryExp},
            {86, binaryExp},
            {88, binaryExp},

            {89, numExp},
            {90, varExp},
            {95, unaryExp},
            {96, unaryExp},
            {97, unaryExp},
        };
    return reduceTable;
}

void semantic::callReduce(size_t id)
{
    auto& table = getReduceTable();
    if(table.find(id) == table.end()) return;
    table[id]();
}
