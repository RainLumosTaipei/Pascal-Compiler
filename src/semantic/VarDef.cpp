#include "semantic/VarDef.h"
#include "semantic/Ast.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "semantic/FuncDef.h"
#include "semantic/SymbolTable.h"

using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;

namespace 
{
    void regisId(string id, Value* v, Type* t)
    {
        getSymbolTable().addVar(id, v, t);
    }

    // 注册全局变量
    void regisGlobal(const std::string& id, bool isConst, Type* type, Constant* initialValue) {
        GlobalVariable *var = new GlobalVariable(
            getModule(), type, isConst, GlobalValue::ExternalLinkage, initialValue, id);
        regisId(id, var, type);
    }

    // 注册局部变量
    void regisLocal(const std::string& id, bool isConst, Type* type, Constant* initialValue) {
        AllocaInst *var = getBuilder().CreateAlloca(type, nullptr, id);
        // 将 initialValue 的值存储到 var 所指向的内存位置
        getBuilder().CreateStore(initialValue, var);
        regisId(id, var, type);
    }
}

namespace
{
    template <typename T, unsigned BitWidth = 32, bool IsSigned = true>
    void regisVarWithValue(const std::string& id,  const string& val, bool isConstant, bool isGlobal) {
        LLVMContext& context = getContext();
        Type* type;
        Constant* zero;

        if constexpr (std::is_same_v<T, float>) {
            type = Type::getFloatTy(context);
            zero = ConstantFP::get(type, stod(val));
        } else {
            IntegerType* intType = IntegerType::get(context, BitWidth);
            type = intType;
            zero = ConstantInt::get(intType, stoi(val), IsSigned);
        }

        if (isGlobal) 
            return regisGlobal(id, isConstant, type, zero);
        return regisLocal(id, isConstant, type, zero);
    }

    [[maybe_unused]]
    void regisVarIntWithValue(const std::string& id, const string& val, bool isGlobal) {
        regisVarWithValue<int>(id, val, false, isGlobal);
    }

    [[maybe_unused]]
    void regisVarRealWithValue(const std::string& id, const string& val, bool isGlobal) {
        regisVarWithValue<float>(id, val, false, isGlobal);
    }

    [[maybe_unused]]
    void regisVarCharWithValue(const std::string& id, const string& val, bool isGlobal) {
        regisVarWithValue<char, 8, false>(id, val, false, isGlobal);
    }

    [[maybe_unused]]
    void regisVarBoolWithValue(const std::string& id, const string& val, bool isGlobal) {
        regisVarWithValue<bool, 1, false>(id, val, false, isGlobal);
    }

    // 常量注册函数
    void regisConstIntWithValue(const std::string& id, const string& val, bool isGlobal) {
        regisVarWithValue<int>(id, val, true, isGlobal);
    }

    void regisConstRealWithValue(const std::string& id, const string& val, bool isGlobal) {
        regisVarWithValue<float>(id, val, true, isGlobal);
    }

    void regisConstCharWithValue(const std::string& id, const string& val, bool isGlobal) {
        regisVarWithValue<char, 8, false>(id, val, true, isGlobal);
    }

    void regisConstBoolWithValue(const std::string& id, const string& val, bool isGlobal) {
        regisVarWithValue<bool, 1, false>(id, val, true, isGlobal);
    }
}



void semantic::regisVar(token::TokenDesc* id, token::TokenDesc* type, bool isGlobal)
{
    auto contValue = cast<Constant>(type->entry.val);
    if (isGlobal) 
        return regisGlobal(id->str, false, type->entry.type, contValue);
    return regisLocal(id->str, false, type->entry.type, contValue);
}

void semantic::regisConstWithValue(const token::TokenDesc* id, token::TokenDesc* cont, bool isGlobal)
{
    switch (cont->token.token)
    {
    case token::TokenState::num:
        if(cont->str.find('.') != string::npos)
            return regisConstRealWithValue(id->str, cont->str, isGlobal);
        return regisConstIntWithValue(id->str, cont->str, isGlobal);
    case token::TokenState::letter:
        return regisConstCharWithValue(id->str, cont->str, isGlobal);
    default:
        throw runtime_error("Unknown const type");
    }
}



void semantic::typeInt(token::TokenDesc* type)
{
    type->entry.type = IntegerType::get(getContext(), 32);
    type->entry.val = ConstantInt::get(type->entry.type, 0, true);
}

void semantic::typeChar(token::TokenDesc* type)
{
    type->entry.type = IntegerType::get(getContext(), 8);
    type->entry.val = ConstantInt::get(type->entry.type, 0, false);
}

void semantic::typeBool(token::TokenDesc* type)
{
    type->entry.type = IntegerType::get(getContext(), 1);
    type->entry.val = ConstantInt::get(type->entry.type, 0, false);
}

void semantic::typeReal(token::TokenDesc* type)
{
    type->entry.type = Type::getFloatTy(getContext());
    type->entry.val = ConstantFP::get(type->entry.type, 0.0);
}

void semantic::typeArray(token::TokenDesc* type, int size)
{
    auto* newTy = ArrayType::get(type->entry.type, size);
    auto contValue = cast<Constant>(type->entry.val);
    auto* newConst =  ConstantArray::get(newTy, vector(size, contValue));
    type->entry.type = newTy;
    type->entry.val = newConst;
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