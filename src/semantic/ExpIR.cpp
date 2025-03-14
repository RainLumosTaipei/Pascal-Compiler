#include "semantic/ExpIR.h"
#include "llvm/IR/Value.h"
#include "semantic/Ast.h"
#include "semantic/TypeIR.h"

using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;

namespace
{
    void convertType(SymbolEntry& l, SymbolEntry& r)
    {
        Type* lt = l.type;
        Type* rt = r.type;
        if (lt == rt) return;

        // bool,char,int to double
        if (lt->isIntegerTy() && rt->isFloatingPointTy())
        {
            l.val = getBuilder().CreateSIToFP(l.val, rt, "int_to_float");
            l.type = rt;
            return;
        }
        if (rt->isIntegerTy() && lt->isFloatingPointTy())
        {
            r.val = getBuilder().CreateSIToFP(r.val, lt, "int_to_float");
            r.type = lt;
            return;
        }

        // bool,char to int
        if (rt->isIntegerTy(32))
        {
            l.val = getBuilder().CreateSExt(l.val, rt, "signed_ext_int");
            l.type = rt;
            return;
        }
        if (lt->isIntegerTy(32))
        {
            r.val = getBuilder().CreateSExt(r.val, lt, "signed_ext_int");
            r.type = lt;
            return;
        }

        // bool to char
        if (lt->isIntegerTy(1) && rt->isIntegerTy(8))
        {
            l.val = getBuilder().CreateZExt(l.val, rt, "bool_to_char");
            l.type = rt;
            return;
        }
        if (rt->isIntegerTy(1) && lt->isIntegerTy(8))
        {
            r.val = getBuilder().CreateZExt(r.val, lt, "bool_to_char");
            r.type = lt;
            return;
        }

        cerr << "Unsupported type conversion" << '\n';
    }
}

// 将指针类型转化为其指向的数，调用 load 并返回指向的数
void semantic::loadIfPointer(SymbolEntry& entry)
{
    // 需要注意其类型本来不是指针类型
    if (entry.val->getType()->isPointerTy() && !entry.type->isPointerTy())
    {
        entry.val = getBuilder().CreateLoad(entry.type, entry.val, entry.val->getName());
    }
}

void semantic::load(Value* id)
{
    getBuilder().CreateLoad(id->getType(), id);
}

// 将 value 存储到变量 id 中， 注意 id 必须是一个指针类型
void semantic::store(token::TokenDesc* exp, token::TokenDesc* ptr)
{
    getBuilder().CreateStore(exp->entry.val, ptr->entry.val);
}

void semantic::num(token::TokenDesc* t)
{
    switch (t->token.token)
    {
    case token::TokenState::num:
        if (t->str.find('.') != string::npos)
        {
            t->entry.val = toReal(t->str);
            t->entry.type = realTy;
            return;
        }
        t->entry.val = toInt(t->str);
        t->entry.type = intTy;
        return;
    default:
        throw runtime_error("Unknown const type");
    }
}

void semantic::boolean(token::TokenDesc* t)
{
    switch (t->save.token)
    {
    case token::TokenState::truly:
        t->entry.val = trueBool;
        t->entry.type = boolTy;
        return;
    case token::TokenState::falsely:
        t->entry.val = falseBool;
        t->entry.type = boolTy;
        return;
    default:
        throw runtime_error("Unknown const type");
    }
}

// 一元操作符
void semantic::unaryOp(token::TokenDesc* op, token::TokenDesc* factor)
{
    if (!factor->entry.val)
        return;

    switch (op->token.token)
    {
    case token::op_neg:
        if (factor->entry.type->isIntegerTy())
        {
            factor->entry.val = getBuilder().CreateNeg(factor->entry.val, "ineg");
            return;
        }
        factor->entry.val = getBuilder().CreateFNeg(factor->entry.val, "fneg");
        return;

    case token::op_not:
        factor->entry.val = getBuilder().CreateNot(factor->entry.val, "not");
        return;

    case token::op_pos:
        return;
    default:
        throw std::runtime_error("Unsupported operator");
    }
}

// 二元操作符
void semantic::binaryOp(token::TokenDesc* op, token::TokenDesc* L, token::TokenDesc* R, token::TokenDesc* ret)
{
    if (!L->entry.val || !R->entry.val)
        return;

    convertType(L->entry, R->entry);
    Type* t = R->entry.type;
    ret->entry.type = R->entry.type;

    switch (op->save.token)
    {
    // calculate operator
    case token::op_add:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateAdd(L->entry.val, R->entry.val, "iadd");
            return;
        }
        ret->entry.val = getBuilder().CreateFAdd(L->entry.val, R->entry.val, "fadd");
        return;

    case token::op_sub:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateSub(L->entry.val, R->entry.val, "isub");
            return;
        }
        ret->entry.val = getBuilder().CreateFSub(L->entry.val, R->entry.val, "fsub");
        return;
        
    case token::op_mul:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateMul(L->entry.val, R->entry.val, "imul");
            return;
        }
        ret->entry.val = getBuilder().CreateFMul(L->entry.val, R->entry.val, "fmul");
        return;
        
    case token::op_div:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateSDiv(L->entry.val, R->entry.val, "idiv");
            return;
        }
        ret->entry.val = getBuilder().CreateFDiv(L->entry.val, R->entry.val, "fdiv");
        return;
        
    case token::op_mod:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateSRem(L->entry.val, R->entry.val, "imod");
            return;
        }
        ret->entry.val = getBuilder().CreateFRem(L->entry.val, R->entry.val, "fmod");
        return;

    // bits operator
    case token::op_and:
        ret->entry.val = getBuilder().CreateAnd(L->entry.val, R->entry.val, "and");
        return;
    case token::op_or:
        ret->entry.val = getBuilder().CreateOr(L->entry.val, R->entry.val, "or");
        return;
        
    // cmp operator
    case token::op_equal:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateICmpEQ(L->entry.val, R->entry.val, "ieq");
            return;
        }
        ret->entry.val = getBuilder().CreateFCmpOEQ(L->entry.val, R->entry.val, "feq");
        return;
        
    case token::op_not_equ:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateICmpNE(L->entry.val, R->entry.val, "ine");
            return;
        }
        ret->entry.val = getBuilder().CreateFCmpONE(L->entry.val, R->entry.val, "fne");
        return;
        
    case token::op_less:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateICmpSLT(L->entry.val, R->entry.val, "ilt");
            return;
        }
        ret->entry.val = getBuilder().CreateFCmpOLT(L->entry.val, R->entry.val, "flt");
        return;
        
    case token::op_great:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateICmpSGT(L->entry.val, R->entry.val, "igt");
            return;
        }
        ret->entry.val = getBuilder().CreateFCmpOGT(L->entry.val, R->entry.val, "fgt");
        return;

    case token::op_less_equ:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateICmpSLE(L->entry.val, R->entry.val, "ile");
            return;
        }
        ret->entry.val = getBuilder().CreateFCmpOLE(L->entry.val, R->entry.val, "fle");
        return;

    case token::op_great_equ:
        if (t->isIntegerTy())
        {
            ret->entry.val = getBuilder().CreateICmpSGE(L->entry.val, R->entry.val, "ige");
            return;
        }
        ret->entry.val = getBuilder().CreateFCmpOGE(L->entry.val, R->entry.val, "fge");
        return;
        
    default:
        throw std::runtime_error("Unsupported binary operator");
    }
}
