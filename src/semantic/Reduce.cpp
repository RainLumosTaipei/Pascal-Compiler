#include "semantic/Reduce.h"

#include <stack>

#include "semantic/Ast.h"
#include "semantic/ExpIR.h"
#include "syntax/SyntaxCheck.h"
#include "semantic/VarIR.h"
#include "semantic/FuncIR.h"
#include "semantic/SymbolTable.h"
#include "semantic/TypeIR.h"

using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;
using namespace token;

namespace
{
    // func para
    FuncDesc funcDesc;
    // exp/var list
    vector<TokenDesc*> expList;
    vector<TokenDesc*> varList;
    // block
    stack<BlockDesc> blocks;
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
        typeArray(getBackAt(1), getArraySize(4));
        copyBack(desc);
    }

    // period_with_type -> digit .. digit , period_with_type
    void typeArrayDef(TokenDesc* desc)
    {
        typeArray(getBackAt(1), getArraySize(3));
        copyBack(desc);
    }
}

// var const def
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
        regisConst(getBackAt(3), getBackAt(1), isGlobal());
        copyBack(desc);
    }

    // const -> op_neg num
    void negNum(TokenDesc* desc)
    {
        getBackAt(1)->str.insert(0, "-");
        saveBack(desc);
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
        if (funcDesc.isVoid)
            retFunc();
        endFuncBlock();
    }

    // prog_body -> const_defs var_defs sub_prog_def main
    void retMainFunc(TokenDesc* desc)
    {
        retMain();
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

    // factor -> boolean
    void boolExp(TokenDesc* desc)
    {
        semantic::boolean(getBackAt(1));
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
        if (getSymbolTable().findVar(getBackAt(1)))
            return copyBack(desc);
        throw runtime_error("var does not exist");
    }

    // var -> id [ exp_list ]
    void leftArray(TokenDesc* desc)
    {
        auto* id = getBackAt(4);
        if (getSymbolTable().findVar(id))
        {
            getArrayElement(id, expList);
            *desc = *id;
            expList.clear();
            return;
        }
        throw runtime_error("var does not exist");
    }

    // var_list -> var
    // var_list -> var_list , var
    void leftList(TokenDesc* desc)
    {
        varList.push_back(getBackAt(1));
        copyBack(desc);
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

// block
namespace
{
    // if -> key_if
    void ifBlock(TokenDesc* desc)
    {
        blocks.emplace(BlockType::iff);
    }

    // else -> key_else
    // else_part -> null
    void elseBlock(TokenDesc* desc)
    {
        blocks.top().thenBr();
    }

    // stmt_base -> if exp then stmt else_part
    // stmt_base -> while exp do stmt_base
    void mergeBlock(TokenDesc* desc)
    {
        blocks.top().elsBr();
        blocks.pop();
    }

    // stmt_base -> for var := exp to exp do stmt_base
    void mergeForBlock(TokenDesc* desc)
    {
        blocks.top().elsBr(getBackAt(7));
        blocks.pop();
    }

    // if exp then -> if exp key_then
    void thenBlock(TokenDesc* desc)
    {
        blocks.top().condBr(getBackAt(2));
    }

    // while -> key_while
    void whileBlock(TokenDesc* desc)
    {
        blocks.emplace(BlockType::whl);
    }

    // while exp do -> while exp key_do
    void doBlock(TokenDesc* desc)
    {
        blocks.top().condBr(getBackAt(2));
    }

    // for var := exp to -> for var := exp key_to
    void toBlock(TokenDesc* desc)
    {
        *desc = *getBackAt(2);
        store(getBackAt(2), getBackAt(4));
        blocks.emplace(BlockType::fore);
    }

    // for var := exp to exp to-do -> for var := exp to exp key_do
    void todoBlock(TokenDesc* desc)
    {
        blocks.top().condBr(getBackAt(6), getBackAt(2));
    }


    // begin
    void beginBlock(TokenDesc* desc)
    {
        if (isDeepEqu())
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
        if (!isDeepEqu()) return;
        --realDeep;
    }
}

// read write
namespace
{
    void readFunc(TokenDesc* desc)
    {
        callRead(varList);
        varList.clear();
    }

    void writeFunc(TokenDesc* desc)
    {
        callWrite(expList);
        expList.clear();
    }
}

inline ReduceTable& getReduceTable()
{
    static ReduceTable reduceTable{
        
        {4, retMainFunc},
        
        {8, retFuncBlock},
        {9, procDef},
        {10, funcDef},

        {22, constDef},
        {23, constDef},

        {24, saveBack},
        {25, negNum},
        {26, saveBack},
        {27, saveBack},

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
        {46, leftList},
        {47, leftList},

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
        {71, readFunc},
        {72, writeFunc},

        {73, mergeForBlock},
        {74, mergeBlock},
        {75, mergeBlock},
        {76, callFuncNoParenExp},
        {77, callFuncExp},
        {78, callFuncVoidExp},
        {79, elseBlock},

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
        {100, doBlock},
        {101, toBlock},
        {102, ifBlock},
        {103, whileBlock},
        {105, endBlock},
        {106, todoBlock},

        {107, boolExp},
        {108, saveBack},
        {109, saveBack},
        {110, saveBack},
    };
    return reduceTable;
}

TokenDesc* semantic::callReduce(size_t id, TokenState t)
{
    auto& table = getReduceTable();
    auto desc = new TokenDesc(t);
    if (table.find(id) == table.end()) return desc;
    table[id](desc);
    return desc;
}
