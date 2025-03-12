#include "semantic/Reduce.h"

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
    // func para
    FuncDesc funcDesc;
    // exp list
    vector<TokenDesc*> expList;
    size_t realDeep = 1;
}


namespace
{
    void copyBack(TokenDesc* desc)
    {
        auto& waits = syntax::lr::getWaitTokens();
        // token不会赋值, save会赋值
        *desc = *waits.back();
    }

    void saveBack(TokenDesc* desc)
    {
        auto& waits = syntax::lr::getWaitTokens();
        // token不会赋值, save会赋值
        *desc = *waits.back();
        desc->save = waits.back()->token;
    }
    
    TokenDesc* getBackAt(int pos)
    {
        auto& waits = syntax::lr::getWaitTokens();
        auto it = prev(waits.end(), pos);
        return *it;
    }

    int getArraySize(int pos)
    {
        int r = stoi(getBackAt(pos)->str);
        int l = stoi(getBackAt(pos + 2)->str);
        return r - l + 1;
    }

    bool isGlobal()
    {
        return getSymbolTable().deep() == 1;
    }

    bool isDeepEqu()
    {
        return getSymbolTable().deep() == realDeep;
    }

    
    
}



// type
namespace
{
    // type_base -> integer
    void typeIntDef(TokenDesc* desc)
    {
        typeInt(getBackAt(1));
        copyBack(desc);
    }

    // type_base -> char
    void typeCharDef(TokenDesc* desc)
    {
        typeChar(getBackAt(1));
        copyBack(desc);
    }

    // type_base -> boolean
    void typeBoolDef(TokenDesc* desc)
    {
        typeBool(getBackAt(1));
        copyBack(desc);
    }

    // type_base -> real
    void typeRealDef(TokenDesc* desc)
    {
        typeReal(getBackAt(1));
        copyBack(desc);
    }

    // period_with_type -> digit .. digit ] key_of type_base
    void typeArrayBaseDef(TokenDesc* desc)
    {
        typeArray(getBackAt(1),  getArraySize(4));
        copyBack(desc);
    }

    // period_with_type -> digit .. digit , period_with_type
    void typeArrayDef(TokenDesc* desc)
    {
        typeArray(getBackAt(1), getArraySize(3));
        copyBack(desc);
    }
}

// var const
namespace
{
    // var_with_type -> id : type
    // var_with_type -> id , var_with_type
    void varDef(TokenDesc* desc)
    {
        regisVar(getBackAt(3), getBackAt(1), isGlobal());
        // 复制类型信息
        copyBack(desc);
    }

    // const_def -> id = const
    // const_def -> const_def ; id = const 
    void constDef(TokenDesc* desc)
    {
        regisConstWithValue(getBackAt(3), getBackAt(1), isGlobal());
        copyBack(desc);
    }

    // const -> op_neg num
    void negNum(TokenDesc* desc)
    {
        getBackAt(1)->str.insert(0, "-");
        copyBack(desc);
    }
    

}

// func
namespace
{
    // id_with_type -> id : type_base
    // id_with_type -> id , id_with_type
    void addPara(TokenDesc* desc)
    {
        funcDesc.paraName.push_back(getBackAt(3));
        funcDesc.paraType.push_back(getBackAt(1));
        copyBack(desc);
    }

    // sub_prog_head -> key_func idf formal_para : type_base
    void funcDef(TokenDesc* desc)
    {
        funcDesc.rev = getBackAt(1);
        funcDesc.name = getBackAt(4);
        funcDesc.isVoid = false;
        regisFunc(funcDesc);
        funcDesc.paraName.clear();
        funcDesc.paraType.clear();

    }

    // sub_prog_head -> key_proc idf formal_para
    void procDef(TokenDesc* desc)
    {
        funcDesc.name = getBackAt(2);
        funcDesc.isVoid = true;
        regisFunc(funcDesc);
        funcDesc.paraName.clear();
        funcDesc.paraType.clear();
    }

    // sub_prog_body -> const_defs var_defs main
    void retFuncBlock(TokenDesc* desc)
    {
        endFuncBlock();
        if(funcDesc.isVoid)
            retFunc();
    }
}

// exp
namespace
{
    // factor -> var
    void varExp(TokenDesc* desc)
    {
        // 此时应该是变量的右值，加载其值
        loadIfPointer(getBackAt(1)->entry);
        copyBack(desc);
    }

    // factor -> num
    void numExp(TokenDesc* desc)
    {
        semantic::num(getBackAt(1));
        copyBack(desc);
    }

    // factor -> ( exp )
    void parenExp(TokenDesc* desc)
    {
        *desc = *getBackAt(2);
    }

    // var -> id
    void leftVar(TokenDesc* desc)
    {
        if(getSymbolTable().findVar(getBackAt(1)))
            return copyBack(desc);
        throw runtime_error("var does not exist");
    }

    // var -> id [ exp_list ]
    void leftArray(TokenDesc* desc)
    {
        auto* id = getBackAt(4);
        if(getSymbolTable().findVar(id))
        {
            getArrayElement(id, expList);
            *desc = *id;
            expList.clear();
            return;
        }
        throw runtime_error("var does not exist");
    }

    // stmt_base -> var := exp 
    void assignExp(TokenDesc* desc)
    {
        copyBack(desc);
        // 将 exp 右值存入 var 左值地址
        store(getBackAt(1), getBackAt(3));
    }

    // stmt_base -> idf := exp 
    void retExp(TokenDesc* desc)
    {
        copyBack(desc);
        retFunc(getBackAt(1));
    }

    // factor -> not factor
    // factor -> op_neg factor
    // factor -> op_pos factor
    void unaryExp(TokenDesc* desc)
    {
        unaryOp(getBackAt(2), getBackAt(1));
        // 不改变类型
        copyBack(desc);
    }

    // exp -> exp op_cmp sub_exp
    // sub_exp -> sub_exp op_add_sub term
    // term -> term op_div_mul factor
    void binaryExp(TokenDesc* desc)
    {
        copyBack(desc);
        binaryOp(getBackAt(2), getBackAt(3), getBackAt(1), desc);
    }

    // exp_list -> exp
    // exp_list -> exp_list , exp
    void addExpList(TokenDesc* desc)
    {
        expList.push_back(getBackAt(1));
        copyBack(desc);
    }

    // proc_call -> idf ( exp_list )
    void callFuncExp(TokenDesc* desc)
    {
        auto* idf = getBackAt(4);
        callFunc(idf, expList);
        *desc = *idf;
        expList.clear();
    }

    // factor -> idf (  )
    void callFuncVoidExp(TokenDesc* desc)
    {
        auto* idf = getBackAt(3);
        callFunc(idf, expList);
        *desc = *idf;
    }

    // factor -> idf
    void callFuncNoParenExp(TokenDesc* desc)
    {
        auto* idf = getBackAt(1);
        callFunc(idf, expList);
        *desc = *idf;
    }
    
}


namespace
{
    // if -> key_if
    void ifBlock(TokenDesc* desc)
    {
        
    }

    void elseBlock(TokenDesc* desc)
    {
        
    }
    void thenBlock(TokenDesc* desc)
    {
        
    }

    // begin
    void beginBlock(TokenDesc* desc)
    {
        if(isDeepEqu())
        {
            startFuncBlock("entry");
            ++realDeep;
            return;
        }
        ++realDeep;
    }

    // end
    void endBlock(TokenDesc* desc)
    {
        getSymbolTable().leaveScope();
        --realDeep;
    }
}

inline ReduceTable& getReduceTable()
{
    static ReduceTable reduceTable{

            {8, retFuncBlock},
            {9, procDef},
            {10, funcDef},
        
            {22, constDef},
            {23, constDef},

            {24, copyBack},
            {25, negNum},
            {26, copyBack},
            {27, copyBack},

            {32, varDef},
            {33, varDef},
            {34, copyBack},
            {35, copyBack},
            
            {36, typeIntDef},
            {37, typeRealDef},
            {38, typeCharDef},
            {39, typeBoolDef},
            {40, typeArrayDef},
            {41, typeArrayBaseDef},
        
            {42, addPara},
            {43, addPara},
            {44, leftVar},
            {45, leftArray},

            {48, saveBack},
            {49, saveBack},
            {50, saveBack},
            {51, saveBack},
            {52, saveBack},
            {53, saveBack},
            {54, saveBack},
            {55, saveBack},
            {56, saveBack},
            {57, saveBack},
            {58, saveBack},
            {59, saveBack},
            {60, saveBack},

            {62, beginBlock},

            {68, copyBack},
            {69, assignExp},
            {70, retExp},

            {76, callFuncNoParenExp},
            {77, callFuncExp},
            {78, callFuncVoidExp},

            {81, addExpList},
            {82, addExpList},
            {83, copyBack},
            {84, binaryExp},
            {85, copyBack},
            {86, binaryExp},
            {87, copyBack},
            {88, binaryExp},

            {89, numExp},
            {90, varExp},
            {91, parenExp},
            {92, callFuncExp},
            {93, callFuncVoidExp},
            {94, callFuncNoParenExp},
            {95, unaryExp},
            {96, unaryExp},
            {97, unaryExp},

            
            {98, thenBlock},
            {99, elseBlock},
            {102, ifBlock},
            {105, endBlock},
        };
    return reduceTable;
}

TokenDesc* semantic::callReduce(size_t id, TokenState t)
{
    auto& table = getReduceTable();
    TokenDesc* desc = new TokenDesc(t);
    if(table.find(id) == table.end()) return desc;
    table[id](desc);
    return desc;
}
