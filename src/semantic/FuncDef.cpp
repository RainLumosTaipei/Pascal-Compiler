#include "semantic/FuncDef.h"

#include <stack>

#include "semantic/Ast.h"
#include "semantic/SymbolTable.h"
#include "semantic/VarDef.h"

using namespace std;
using namespace semantic;
using namespace ast;
using namespace llvm;

namespace
{
    stack<Function*>& getFuncScope(){
        static stack<Function*> funcScope;
        // 添加 main 函数
        if(funcScope.empty())
        {
            FunctionType *MainFT = FunctionType::get(Type::getInt32Ty(getContext()), false);
            Function *MainF = Function::Create(MainFT, Function::ExternalLinkage, "main", getModule());
            funcScope.push(MainF);
        }
        return funcScope;
    }

    
    void allocParas(const FuncDesc& desc)
    {
        for (size_t i = 0; i < desc.paraName.size(); ++i)
        {
            regisVar(desc.paraName[i], desc.paraType[i], false);
        }
    }
    
    
}


void semantic::startFuncBlock(const string& name)
{
    BasicBlock* BB = BasicBlock::Create(getContext(), name, getFuncScope().top());
    getBuilder().SetInsertPoint(BB);
    getSymbolTable().enterScope();
}



void semantic::endFuncBlock()
{
    getFuncScope().pop();
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
    getFuncScope().push(func);

    startFuncBlock("entry");
    
    int id = 0;
    for(auto& arg : func->args())
    {
        auto& argName = desc.paraName[id++]->str;
        arg.setName(argName);
    }

    allocParas(desc);
}

void semantic::callFunc(token::TokenDesc* idf, const vector<token::TokenDesc*>& exps)
{
    Function* func = getModule().getFunction(idf->str);
    vector<Value*> args;
    for (auto it : exps)
    {
        args.push_back(it->entry.val);
    }
    CallInst *call = getBuilder().CreateCall(func, args);
    idf->entry.val = call;
    idf->entry.type = call->getType();
}

        