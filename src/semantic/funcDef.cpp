#include "semantic/funcDef.h"

#include <complex>

#include "semantic/Ast.h"

using namespace std;
using namespace semantic;
using namespace ast;
using namespace llvm;

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
    
    Function *func = Function::Create(funcTy, Function::ExternalLinkage, desc.name, getModule());
    
    int id = 0;
    for(auto& arg : func->args())
    {
        arg.setName(desc.paras[id++].second);
    }
}



        