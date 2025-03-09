#include "semantic/Ast.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <iostream>

using namespace llvm;
using namespace std;

inline LLVMContext& ast::getContext()
{
    static LLVMContext context;
    return context;
}

inline Module& ast::getModule()
{
    static Module module("Pascal JIT", getContext());
    return module;
}

inline IRBuilder<>& ast::getBuilder()
{
    static IRBuilder<> builder(getContext());
    return builder;
}


Value* logError(const char* Str) {
    cerr << Str << '\n';
    return nullptr;
}

void convertType(Value* l, Value* r) {
    Type* lt = l->getType();
    Type* rt = r->getType();
    if (lt == rt) return;

    auto& builder = ast::getBuilder();

    // bool,char,int to double
    if (lt->isIntegerTy() && rt->isFloatingPointTy()) {
        l =  builder.CreateSIToFP(l, rt, "int_to_float");
        return;
    }
    if (rt->isIntegerTy() && lt->isFloatingPointTy()) {
        r =  builder.CreateSIToFP(r, lt, "int_to_float");
        return;
    }

    // bool,char to int
    if (rt->isIntegerTy(32)) {
        l =  builder.CreateSExt(l, rt, "signed_ext_int");
        return;
    }
    if (lt->isIntegerTy(32)) {
        r =  builder.CreateSExt(r, lt, "signed_ext_int");
        return;
    }

    // bool to char
    if (lt->isIntegerTy(1) && rt->isIntegerTy(8)) {
        l = builder.CreateZExt(l, rt, "bool_to_char");
        return;
    }
    if (rt->isIntegerTy(1) && lt->isIntegerTy(8)) {
        r = builder.CreateZExt(r, lt, "bool_to_char");
        return;
    }
    
    cerr << "Unsupported type conversion" << '\n';
    return;
}

// real 常数
Value* ast::RealExprAST::codegen() {
    return ConstantFP::get(*context, APFloat(v));
}

// integer 常数
Value* ast::IntExprAST::codegen() {
    return ConstantInt::get(*context, APInt(32, v, true));
}

// char 常数
Value* ast::CharExprAST::codegen() {
    return ConstantInt::get(*context, APInt(8, v));
}

// bool 常数
Value* ast::BoolExprAST::codegen() {
    return ConstantInt::get(*context, APInt(1, v));
}

// id 变量
Value* ast::VariableExprAST::codegen() {
    Value* V = idTable[name];
    if (!V)
        return logError("Unknown variable name");
    return V;
}

// 一元操作符
Value* ast::UnaryOpExprAST::codegen()
{
    Value* V = v->codegen();
    if (!V)
        return nullptr;
    
    switch (op)
    {
    case token::op_neg:
        if(V->getType()->isIntegerTy())
            return builder->CreateNeg(V,"int neg");
        return builder->CreateFNeg(V, "float neg");
        
    case token::op_not:
        return builder->CreateNot(V, "not");

    default:
        return logError("invalid binary operator");
    }
    
}

// 二元操作符
Value* ast::BinaryOpExprAST::codegen() {
    Value* L = l->codegen();
    Value* R = r->codegen();
    if (!L || !R)
        return nullptr;
    
    convertType(L, R);
    Type* t = R->getType();
    
    switch (op) {
    // calculate operator
    case token::op_add:
        if(t->isIntegerTy())
            return builder->CreateAdd(L, R, "int add");
        return builder->CreateFAdd(L, R, "float add");
        
    case token::op_sub:
        if(t->isIntegerTy())
            return builder->CreateSub(L, R, "int sub");
        return builder->CreateFSub(L, R, "float sub");
        
    case token::op_mul:
        if(t->isIntegerTy())
            return builder->CreateMul(L, R, "int mul");
        return builder->CreateFMul(L, R, "float mul");
        
    case token::op_div:
        if(t->isIntegerTy())
            return builder->CreateSDiv(L, R, "signed int div");
        return builder->CreateFDiv(L, R, "float sub");
        
    case token::op_mod:
        if(t->isIntegerTy())
            return builder->CreateSRem(L, R, "signed int mod");
        return builder->CreateFRem(L, R, "float mod");

    // bits operator
    case token::op_and:
        return builder->CreateAnd(L, R, "and");
    case token::op_or:
        return builder->CreateOr(L, R, "or");

    // cmp operator
    case token::op_equal:
        if(t->isIntegerTy())
            return builder->CreateICmpEQ(L, R, "int =");
        return builder->CreateFCmpOEQ(L, R, "float =");
        
    case token::op_not_equ:
        if(t->isIntegerTy())
            return builder->CreateICmpNE(L, R, "int <>");
        return builder->CreateFCmpONE(L, R, "float <>");
        
    case token::op_less:
        if(t->isIntegerTy())
            return builder->CreateICmpSLT(L, R, "signed int <");
        return builder->CreateFCmpOLT(L, R, "float <");
        
    case token::op_great:
        if(t->isIntegerTy())
            return builder->CreateICmpSGT(L, R, "signed int >");
        return builder->CreateFCmpOGT(L, R, "float >");
        
    case token::op_less_equ:
        if(t->isIntegerTy())
            return builder->CreateICmpSLE(L, R, "signed int <=");
        return builder->CreateFCmpOLE(L, R, "float <=");
        
    case token::op_great_equ:
        if(t->isIntegerTy())
            return builder->CreateICmpSGE(L, R, "signed int >=");
        return builder->CreateFCmpOGE(L, R, "float >=");
        
    default:
        return logError("invalid binary operator");
    }
}

// 函数调用
Value* ast::CallExprAST::codegen() {
    Function* CalleeF = module->getFunction(Callee);
    if (!CalleeF)
        return logError("Unknown function referenced");


    if (CalleeF->arg_size() != Args.size())
        return logError("Incorrect # arguments passed");

    vector<Value*> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
        ArgsV.push_back(Args[i]->codegen());
        if (!ArgsV.back())
            return nullptr;
    }
    return builder->CreateCall(CalleeF, ArgsV, "call");
}

// 函数声明
Function* ast::PrototypeAST::codegen() {
    vector<Type*> Doubles(args.size(), Type::getDoubleTy(*context));
    FunctionType* FT =
        FunctionType::get(Type::getDoubleTy(*context), Doubles, false);

    Function* F =
        Function::Create(FT, Function::ExternalLinkage, name, module.get());
    
    unsigned Idx = 0;
    for (auto& Arg : F->args())
        Arg.setName(args[Idx++]);

    return F;
}

// 函数定义
Function* ast::FunctionAST::codegen() {
    // First, check for an existing function from a previous 'extern' declaration.
    Function* TheFunction = module->getFunction(proto->getName());

    if (!TheFunction)
        TheFunction = proto->codegen();

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    BasicBlock* BB = BasicBlock::Create(*context, "entry", TheFunction);
    builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    idTable.clear();
    for (auto& Arg : TheFunction->args())
        idTable[std::string(Arg.getName())] = &Arg;

    if (Value* RetVal = body->codegen()) {
        // Finish off the function.
        builder->CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

        return TheFunction;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    return nullptr;
}