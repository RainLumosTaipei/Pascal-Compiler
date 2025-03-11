#include "semantic/Exp.h"
#include "llvm/IR/Value.h"
#include "semantic/Ast.h"
#include "semantic/FuncDef.h"
#include "semantic/SymbolTable.h"


using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;

namespace
{
    void convertType(SymbolEntry& l, SymbolEntry& r) {
        Type* lt = l.type;
        Type* rt = r.type;
        if (lt == rt) return;
        
        // bool,char,int to double
        if (lt->isIntegerTy() && rt->isFloatingPointTy()) {
            l.val =  getBuilder().CreateSIToFP(l.val, rt, "int_to_float");
            l.type = rt;
            return;
        }
        if (rt->isIntegerTy() && lt->isFloatingPointTy()) {
            r.val =  getBuilder().CreateSIToFP(r.val, lt, "int_to_float");
            r.type = lt;
            return;
        }

        // bool,char to int
        if (rt->isIntegerTy(32)) {
            l.val =  getBuilder().CreateSExt(l.val, rt, "signed_ext_int");
            l.type = rt;
            return;
        }
        if (lt->isIntegerTy(32)) {
            r.val =  getBuilder().CreateSExt(r.val, lt, "signed_ext_int");
            r.type = lt;
            return;
        }

        // bool to char
        if (lt->isIntegerTy(1) && rt->isIntegerTy(8)) {
            l.val = getBuilder().CreateZExt(l.val, rt, "bool_to_char");
            l.type = rt;
            return;
        }
        if (rt->isIntegerTy(1) && lt->isIntegerTy(8)) {
            r.val = getBuilder().CreateZExt(r.val, lt, "bool_to_char");
            r.type = lt;
            return;
        }
    
        cerr << "Unsupported type conversion" << '\n';
    }

    // real 常数
    Value* realNum(float v) {
        return ConstantFP::get(getContext(), APFloat(v));
    }

    // integer 常数
    Value* intNum(int v) {
        return ConstantInt::get(getContext(), APInt(32, v, true));
    }

    // char 常数
    Value* charNum(char v) {
        return ConstantInt::get(getContext(), APInt(8, v));
    }

    // bool 常数
    Value* boolNum(bool v) {
        return ConstantInt::get(getContext(), APInt(1, v));
    }
}

void semantic::loadIfPointer(SymbolEntry& entry)
{
    if (entry.val->getType()->isPointerTy() && !entry.type->isPointerTy())
    {
        entry.val = getBuilder().CreateLoad(entry.type, entry.val, "load_" + entry.val->getName());
    }
}

void semantic::load(Value* id)
{
    getBuilder().CreateLoad(id->getType(),id);
}

void semantic::store(Value* value, Value* id)
{
    getBuilder().CreateStore(value, id);
}

void semantic::num(const token::TokenDesc* t, SymbolEntry& entry)
{
    switch (t->token.token)
    {
    case token::TokenState::num:
        if(t->value.find('.') != string::npos)
        {
            entry.val = realNum(stof(t->value));
            entry.type =  Type::getFloatTy(getContext());
            return;
        }
        entry.val = intNum(stoi(t->value));
        entry.type = IntegerType::get(getContext(), 32);
        return ;
    default:
        throw runtime_error("Unknown const type");
    }
}

// 一元操作符
void semantic::unaryOp(token::TokenState op, SymbolEntry& entry)
{
    if (!entry.val)
        return ;
    loadIfPointer(entry);
    
    switch (op)
    {
    case token::op_neg:
        if(entry.type->isIntegerTy())
        {
            entry.val = getBuilder().CreateNeg(entry.val,"ineg");
            return;
        }
        entry.val = getBuilder().CreateFNeg(entry.val, "fneg");
        return;
        
    case token::op_not:
        entry.val = getBuilder().CreateNot(entry.val, "not");
        return;

    case token::op_pos:
        return ;
    default:
        throw std::runtime_error("Unsupported operator");
    }
    
}

// 二元操作符
void semantic::binaryOp (token::TokenState op,  SymbolEntry& L, SymbolEntry& R, SymbolEntry& entry){
    if (!L.val || !R.val)
        return ;

    // 消除指针
    loadIfPointer(L);
    loadIfPointer(R);
    
    convertType(L, R);
    Type* t = R.type;
    entry.type = R.type;
    
    switch (op) {
    // calculate operator
    case token::op_add:
        if(t->isIntegerTy())
        {
            entry.val = getBuilder().CreateAdd(L.val, R.val, "iadd");
            return;
        }
        entry.val = getBuilder().CreateFAdd(L.val, R.val, "fadd");
        return;
        
    case token::op_sub:
        if(t->isIntegerTy())
        {
            entry.val =getBuilder().CreateSub(L.val, R.val, "isub");
            return;
        }
        entry.val =getBuilder().CreateFSub(L.val, R.val, "fsub");
        return;
    case token::op_mul:
        if(t->isIntegerTy())
        {
            entry.val =getBuilder().CreateMul(L.val, R.val, "imul");
            return;
        }
        entry.val =getBuilder().CreateFMul(L.val, R.val, "fmul");
        return;
    case token::op_div:
        if(t->isIntegerTy())
        {
            entry.val =getBuilder().CreateSDiv(L.val, R.val, "idiv");
            return;
        }
        entry.val =getBuilder().CreateFDiv(L.val, R.val, "fdiv");
        return;
    case token::op_mod:
        if(t->isIntegerTy())
        {
            entry.val =getBuilder().CreateSRem(L.val, R.val, "imod");
            return;
        }
        entry.val =getBuilder().CreateFRem(L.val, R.val, "fmod");
        return;

    // bits operator
    case token::op_and:
        entry.val =getBuilder().CreateAnd(L.val, R.val, "and");
        return;
    case token::op_or:
        entry.val =getBuilder().CreateOr(L.val, R.val, "or");
        return;
    // cmp operator
    case token::op_equal:
        if(t->isIntegerTy())
        {
            entry.val =getBuilder().CreateICmpEQ(L.val, R.val, "ieq");
            return;
        }
        entry.val =getBuilder().CreateFCmpOEQ(L.val, R.val, "feq");
        return;
    case token::op_not_equ:
        if(t->isIntegerTy())
        {
            entry.val =getBuilder().CreateICmpNE(L.val, R.val, "ine");
            return;
        }
         entry.val =getBuilder().CreateFCmpONE(L.val, R.val, "fne");
        return;
    case token::op_less:
        if(t->isIntegerTy())
        {
            entry.val =getBuilder().CreateICmpSLT(L.val, R.val, "ilt");
            return;
        }
        entry.val =getBuilder().CreateFCmpOLT(L.val, R.val, "flt");
        return;
    case token::op_great:
        if(t->isIntegerTy())
        {
            entry.val =getBuilder().CreateICmpSGT(L.val, R.val, "igt");
            return;
        }
        entry.val =getBuilder().CreateFCmpOGT(L.val, R.val, "fgt");
        return;
        
    case token::op_less_equ:
        if(t->isIntegerTy())
        {
            entry.val =getBuilder().CreateICmpSLE(L.val, R.val, "ile");
            return;
        }
        entry.val =getBuilder().CreateFCmpOLE(L.val, R.val, "fle");
        return;
        
    case token::op_great_equ:
        if(t->isIntegerTy())
        {
            entry.val =getBuilder().CreateICmpSGE(L.val, R.val, "ige");
            return;
        }
         entry.val =getBuilder().CreateFCmpOGE(L.val, R.val, "fge");
        return;
    default:
        throw std::runtime_error("Unsupported binary operator");
    }
}