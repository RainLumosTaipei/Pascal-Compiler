#include "semantic/FuncDef.h"
#include "semantic/Ast.h"
#include "semantic/SymbolTable.h"
#include "semantic/VarDef.h"

using namespace std;
using namespace semantic;
using namespace ast;
using namespace llvm;

Function* semantic::getFunc(const string& name)
{
    Function* func = getModule().getFunction(name);
    return  func;
}

namespace
{

    void startBlock(Function* func)
    {
        BasicBlock* BB = BasicBlock::Create(getContext(), "entry", func);
        getBuilder().SetInsertPoint(BB);
        getSymbolTable().enterScope();
    }

    void allocParas(const FuncDesc& desc)
    {
        for (size_t i = 0; i < desc.paraName.size(); ++i)
        {
            regisVar(desc.paraName[i], desc.paraType[i], false);
        }
    }
}



void semantic::retFunc(token::TokenDesc* ret)
{
    getBuilder().CreateRet(ret->entry.val);
}

void semantic::retFunc()
{
    getBuilder().CreateRetVoid();
}


void semantic::regisFunc(const FuncDesc& desc)
{
    FunctionType *funcTy;
    Type* retTy;

    // 返回值是否为空
    if(desc.isVoid)
        retTy = Type::getVoidTy(getContext());
    else
        retTy = desc.rev->entry.type;

    // 参数是否为空
    if (!desc.paraType.empty())
    {
        vector<Type*> paraTy;
        for (auto it : desc.paraType)
            paraTy.push_back(it->entry.type);
        funcTy = FunctionType::get(retTy, paraTy, false);
    }
    else
        funcTy = FunctionType::get(retTy, false);
    
    Function *func = Function::Create(funcTy, Function::ExternalLinkage, desc.name->str, getModule());

    startBlock(func);
    
    int id = 0;
    for(auto& arg : func->args())
    {
        auto& argName = desc.paraName[id++]->str;
        arg.setName(argName);
    }

    allocParas(desc);
}



        