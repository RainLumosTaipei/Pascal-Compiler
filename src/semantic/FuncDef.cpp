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
    Type* getParaType(token::TokenState t)
    {
        switch (t)
        {
        case token::type_int:
            return IntegerType::get(getContext(), 32);
        case token::type_bool:
            return IntegerType::get(getContext(), 1);
        case token::type_char:
            return IntegerType::get(getContext(), 8);
        case token::type_real:
            return Type::getFloatTy(getContext());
        case token::null:
            return Type::getVoidTy(getContext());
        default:
            throw std::runtime_error("Invalid token type");
        }
    }

    void startBlock(Function* func)
    {
        BasicBlock* BB = BasicBlock::Create(getContext(), "entry", func);
        getBuilder().SetInsertPoint(BB);
        getSymbolTable().enterScope();
    }

    void allocParas(const FuncDesc& desc)
    {
        if (!desc.paras.empty())
            for(auto& it : desc.paras)
                regisVar(it);
    }
}



void semantic::retFunc(Value* ret)
{
    getBuilder().CreateRet(ret);
}

void semantic::retFunc()
{
    getBuilder().CreateRetVoid();
}


void semantic::regisFunc(const FuncDesc& desc)
{
    FunctionType *funcTy;
    if (!desc.paras.empty())
    {
        vector<Type*> paramTypes;
        for(auto& it : desc.paras)
            paramTypes.push_back(getParaType(it.first));
        funcTy = FunctionType::get(getParaType(desc.rev), paramTypes, false);
    }
    else
        funcTy = FunctionType::get(getParaType(desc.rev), false);
    
    Function *func = Function::Create(funcTy, Function::ExternalLinkage, desc.name->value, getModule());

    startBlock(func);
    
    int id = 0;
    for(auto& arg : func->args())
    {
        auto& argName = desc.paras[id++].second->value;
        arg.setName(argName);
    }

    allocParas(desc);
}



        