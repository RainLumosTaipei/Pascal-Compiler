#include "semantic/FuncIR.h"

#include <stack>

#include <llvm/IR/Verifier.h>

#include "semantic/Ast.h"
#include "semantic/ExpIR.h"
#include "semantic/SymbolTable.h"
#include "semantic/TypeIR.h"
#include "semantic/VarIR.h"

using namespace std;
using namespace semantic;
using namespace ast;
using namespace llvm;

namespace
{
    void regisRead()
    {
        Type* ptrTy = PointerType::get(intTy, 0);
        std::vector<Type*> args = {ptrTy};
        FunctionType* readTy = FunctionType::get(voidTy, args, true);

        Function::Create(readTy, Function::ExternalLinkage, "scanf", getModule());
    }

    void regisWrite()
    {
        Type* ptrTy = PointerType::get(intTy, 0);
        std::vector<Type*> args = {ptrTy};
        FunctionType* writeIntTy = FunctionType::get(voidTy, args, true);
        Function::Create(writeIntTy, Function::ExternalLinkage, "printf", getModule());
    }

    stack<Function*>& getFuncScope()
    {
        static stack<Function*> funcScope;
        return funcScope;
    }
}

void semantic::regisProgram(token::TokenDesc* desc)
{
    while (!getFuncScope().empty())  getFuncScope().pop();
    
    FunctionType* MainType = FunctionType::get(intTy, false);
    Function* MainFunc = Function::Create(MainType, Function::ExternalLinkage, desc->str, getModule());
    getFuncScope().push(MainFunc);

    regisRead();
    regisWrite();
}

void semantic::startFuncBlock(const string& name)
{
    BasicBlock* BB = BasicBlock::Create(getContext(), name, getFuncScope().top());
    getBuilder().SetInsertPoint(BB);
    getSymbolTable().enterScope();
}

void semantic::endFuncBlock()
{
    auto* func = getFuncScope().top();
    verifyFunction(*func);
    getFPM().run(*func, getFAM());
    getFuncScope().pop();
    getSymbolTable().leaveScope();
}

BlockDesc::BlockDesc(BlockType type): type(type)
{
    entry = BasicBlock::Create(getContext(), "entry", getFuncScope().top());
    then = BasicBlock::Create(getContext(), "then", getFuncScope().top());
    if (type == iff)
        els = BasicBlock::Create(getContext(), "else", getFuncScope().top());
    merge = BasicBlock::Create(getContext(), "merge", getFuncScope().top());

    getBuilder().CreateBr(entry);
    getBuilder().SetInsertPoint(entry);
}

void BlockDesc::condBr(token::TokenDesc* desc)
{
    if (type == iff)
    {
        getBuilder().CreateCondBr(desc->entry.val, then, els);
        getBuilder().SetInsertPoint(then);
        // enter then
        getSymbolTable().enterScope();
        return;
    }
    getBuilder().CreateCondBr(desc->entry.val, then, merge);
    getBuilder().SetInsertPoint(then);
    // enter then
    getSymbolTable().enterScope();
}

void BlockDesc::condBr(token::TokenDesc* id, token::TokenDesc* exp)
{
    token::TokenDesc op(token::op_less_equ);
    op.save.token = token::op_less_equ;
    Value* load = getBuilder().CreateLoad(id->entry.type, id->entry.val, id->entry.val->getName());
    token::TokenDesc loadDesc = *id;
    loadDesc.entry.val = load;
    binaryOp(&op, &loadDesc, exp, exp);
    condBr(exp);
}

void BlockDesc::thenBr()
{
    if (type == iff)
    {
        getBuilder().CreateBr(merge);
        // leave then
        getSymbolTable().leaveScope();
        getBuilder().SetInsertPoint(els);
        // enter else
        getSymbolTable().enterScope();
        return;
    }
}

void BlockDesc::elsBr()
{
    if (type == iff)
    {
        getBuilder().CreateBr(merge);
        // leave else
        getSymbolTable().leaveScope();
        getBuilder().SetInsertPoint(merge);
        return;
    }
    // leave then
    getSymbolTable().leaveScope();
    getBuilder().CreateBr(entry);
    getBuilder().SetInsertPoint(merge);
}

void BlockDesc::elsBr(token::TokenDesc* desc)
{
    Value* load = getBuilder().CreateLoad(desc->entry.type, desc->entry.val, desc->entry.val->getName());
    Value* one = ConstantInt::get(Type::getInt32Ty(getContext()), 1);
    Value* inc = getBuilder().CreateAdd(load, one, "inc");
    getBuilder().CreateStore(inc, desc->entry.val);
    elsBr();
}

void semantic::retFunc(token::TokenDesc* ret)
{
    getBuilder().CreateRet(ret->entry.val);
}

void semantic::retFunc()
{
    getBuilder().CreateRetVoid();
}

void semantic::retMain()
{
    getBuilder().CreateRet(zeroInt);
}

void semantic::regisFunc(const FuncDesc& desc)
{
    FunctionType* funcTy;
    Type* retTy;

    // 返回值是否为空
    if (desc.isVoid)
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

    Function* func = Function::Create(funcTy, Function::ExternalLinkage, desc.name->str, getModule());
    
    if(desc.isExtern) return;
    
    getFuncScope().push(func);
    startFuncBlock("entry");

    int id = 0;
    for (auto& arg : func->args())
    {
        auto& argName = desc.paraName[id++]->str;
        arg.setName(argName);
        regisArg(arg);
    }
}

void semantic::callFunc(token::TokenDesc* idf, const vector<token::TokenDesc*>& exps)
{
    Function* func = getModule().getFunction(idf->str);
    vector<Value*> args;
    for (auto it : exps)
    {
        args.push_back(it->entry.val);
    }
    CallInst* call = getBuilder().CreateCall(func, args);
    idf->entry.val = call;
    idf->entry.type = call->getType();
}

void semantic::callRead(const vector<token::TokenDesc*>& vars)
{
    string formatStr;
    vector<Value*> args;
    args.push_back(nullptr);
    for (auto it : vars)
    {
        args.push_back(it->entry.val);
        auto* ty = it->entry.type;
        if (ty->isIntegerTy())
            formatStr.append("%d");
        else if (ty->isFloatTy())
            formatStr.append("%f");
    }
    Constant* format = getBuilder().CreateGlobalString(formatStr);
    args[0] = format;
    Function* func = getModule().getFunction("scanf");
    getBuilder().CreateCall(func, args);
}

void semantic::callWrite(const vector<token::TokenDesc*>& exps)
{
    string formatStr;
    vector<Value*> args;
    args.push_back(nullptr);
    for (auto it : exps)
    {
        args.push_back(it->entry.val);
        auto* ty = it->entry.type;
        if (ty->isIntegerTy())
            formatStr.append("%d");
        else if (ty->isFloatTy())
            formatStr.append("%f");
    }
    formatStr.append("\n");
    Constant* format = getBuilder().CreateGlobalString(formatStr);
    args[0] = format;
    Function* func = getModule().getFunction("printf");
    getBuilder().CreateCall(func, args);
}
