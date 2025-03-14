#include "semantic/VarIR.h"
#include "semantic/Ast.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "semantic/SymbolTable.h"
#include "semantic/TypeIR.h"

using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;


namespace
{
    void regisSymbol(string id, Value* v, Type* t)
    {
        getSymbolTable().addVar(id, v, t);
    }

    // 注册全局变量
    void regisGlobal(const std::string& id, bool isConst, Type* type, Constant* init)
    {
        auto var = new GlobalVariable(
            getModule(), type, isConst, GlobalValue::ExternalLinkage, init, id);
        regisSymbol(id, var, type);
    }

    // 注册局部变量
    void regisLocal(const std::string& id, Type* type, Value* init)
    {
        AllocaInst* ptr = getBuilder().CreateAlloca(type, nullptr, id);
        // 将 init 的值存储到 var 所指向的内存位置
        getBuilder().CreateStore(init, ptr);
        regisSymbol(id, ptr, type);
    }

    void regis(const std::string& id, bool isConst, Type* type, Constant* init, bool isGlobal)
    {
        if (isGlobal)
            return regisGlobal(id, isConst, type, init);
        return regisLocal(id, type, init);
    }
}


void semantic::regisVar(token::TokenDesc* id, token::TokenDesc* type, bool isGlobal)
{
    // type有初始化的值0，类型转换为 Constant
    auto contValue = cast<Constant>(type->entry.val);
    if (isGlobal)
        return regisGlobal(id->str, false, type->entry.type, contValue);
    return regisLocal(id->str, type->entry.type, contValue);
}


void semantic::regisArg(Argument& arg)
{
    regisLocal(arg.getName().str(), arg.getType(), &arg);
}

void semantic::regisConst(const token::TokenDesc* id, token::TokenDesc* cont, bool isGlobal)
{
    switch (cont->save.token)
    {
    case token::TokenState::num:
        if (cont->str.find('.') != string::npos)
            return regis(id->str, true, realTy, toReal(cont->str), isGlobal);
        return regis(id->str, true, intTy, toInt(cont->str), isGlobal);
        
    case token::TokenState::letter:
        return regis(id->str, true, charTy, toChar(cont->str), isGlobal);
        
    case token::TokenState::truly:
    case token::TokenState::falsely:
        return regis(id->str, true, boolTy, toBool(cont->str), isGlobal);
        
    default:
        throw runtime_error("Unknown const type");
    }
}

void semantic::getArrayElement(token::TokenDesc* id, const vector<token::TokenDesc*>& pos)
{
    std::vector<Value*> index;
    Type* elementTy = id->entry.type;
    index.push_back(ConstantInt::get(Type::getInt32Ty(getContext()), 0));
    for (auto it : pos)
    {
        // 获取元素类型
        elementTy = elementTy->getArrayElementType();
        index.push_back(it->entry.val);
    }
    id->entry.val = getBuilder().CreateGEP(id->entry.type, id->entry.val, index);
    id->entry.type = elementTy;
}
