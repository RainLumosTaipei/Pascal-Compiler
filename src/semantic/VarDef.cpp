#include "semantic/VarDef.h"
#include "semantic/Ast.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "semantic/SymbolTable.h"

using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;

namespace 
{
    void regisId(string id, Value* v)
    {
        getSymbolTable().addVar(id, v);
    }
    
    void regisGlobal(const std::string& id, bool isConst, Type* type, Constant* initialValue) {
        GlobalVariable *var = new GlobalVariable(
            getModule(), type, isConst, GlobalValue::ExternalLinkage, initialValue, id);
        regisId(id, var);
    }

    void regisLocal(const std::string& id, bool isConst, Type* type, Constant* initialValue) {
        AllocaInst *var = getBuilder().CreateAlloca(type, nullptr, id);
        getBuilder().CreateStore(var, initialValue);
        regisId(id, var);
    }

    // 通用的注册变量函数模板
    template <typename T, unsigned BitWidth = 32, bool IsSigned = true>
    void regisVar(const std::string& id, bool isConstant, bool isGlobal) {
        LLVMContext& context = getContext();
        Type* type;
        Constant* zero;

        if constexpr (std::is_same_v<T, float>) {
            type = Type::getFloatTy(context);
            zero = ConstantFP::get(type, 0.0);
        } else {
            IntegerType* intType = IntegerType::get(context, BitWidth);
            type = intType;
            zero = ConstantInt::get(intType, 0, IsSigned);
        }

        if (isGlobal) 
            return regisGlobal(id, isConstant, type, zero);
        return regisLocal(id, isConstant, type, zero);
    }

    // 变量注册函数
    void regisVarInt(const std::string& id, bool isGlobal) {
        regisVar<int>(id, false, isGlobal);
    }

    void regisVarReal(const std::string& id, bool isGlobal) {
        regisVar<float>(id, false, isGlobal);
    }

    void regisVarChar(const std::string& id, bool isGlobal) {
        regisVar<char, 8, false>(id, false, isGlobal);
    }

    void regisVarBool(const std::string& id, bool isGlobal) {
        regisVar<bool, 1, false>(id, false, isGlobal);
    }

    // 常量注册函数
    [[maybe_unused]]
    void regisConstInt(const std::string& id, bool isGlobal) {
        regisVar<int>(id, true, isGlobal);
    }

    [[maybe_unused]]
    void regisConstReal(const std::string& id, bool isGlobal) {
        regisVar<float>(id, true, isGlobal);
    }

    [[maybe_unused]]
    void regisConstChar(const std::string& id, bool isGlobal) {
        regisVar<char, 8, false>(id, true, isGlobal);
    }

    [[maybe_unused]]
    void regisConstBool(const std::string& id, bool isGlobal) {
        regisVar<bool, 1, false>(id, true, isGlobal);
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

namespace
{
    void regisArray(const std::string& id, bool isConstant, const ArrayDesc& desc, bool isGlobal)
    {
        if (isGlobal)
            return regisGlobal(id, isConstant, desc.arrayType, desc.constant);
        return regisLocal(id, isConstant, desc.arrayType, desc.constant);
    }

    template <typename T, unsigned BitWidth = 32, bool IsSigned = true>
    void getArrayType(int size, ArrayDesc& desc) {
        LLVMContext& context = getContext();
        Type* type;
        Constant* zero;

        if constexpr (std::is_same_v<T, float>) {
            type = Type::getFloatTy(context);
            zero = ConstantFP::get(type, 0.0);
        } else {
            IntegerType* intType = IntegerType::get(context, BitWidth);
            type = intType;
            zero = ConstantInt::get(intType, 0, IsSigned);
        }
        ArrayType* arrayType = ArrayType::get(type, size);
        desc.arrayType = arrayType;
        desc.constant = ConstantArray::get(arrayType, vector(size, zero));
    }

    void getArrayTypeInt(int size, ArrayDesc& desc) {
        getArrayType<int>(size, desc);
    }
    
    void getArrayTypeChar(int size, ArrayDesc& desc) {
        getArrayType<char, 8>(size, desc);
    }
    
    void getArrayTypeReal(int size, ArrayDesc& desc) {
        getArrayType<float>(size, desc);
    }
    
    void getArrayTypeBool(int size, ArrayDesc& desc) {
        getArrayType<bool, 1>(size, desc);
    }
}

void semantic::regisVar(const token::TokenDesc* name, token::TokenDesc* type, bool isGlobal)
{
    switch (type->token.token)
    {
    case token::TokenState::type_int:
        regisVarInt(name->value, isGlobal);
        break;
    case token::TokenState::type_real:
        regisVarReal(name->value, isGlobal);
        break;
    case token::TokenState::type_char:
        regisVarChar(name->value, isGlobal);
        break;
    case token::TokenState::type_bool:
        regisVarBool(name->value, isGlobal);
        break;
    default:
        throw runtime_error("Unknown var type");
    }
}

void semantic::regisConstWithValue(const token::TokenDesc* name, token::TokenDesc* t, bool isGlobal)
{
    switch (t->token.token)
    {
    case token::TokenState::num:
        if(t->value.find('.') != string::npos)
            return regisConstRealWithValue(name->value, t->value, isGlobal);
        return regisConstIntWithValue(name->value, t->value, isGlobal);
    case token::TokenState::letter:
        return regisConstCharWithValue(name->value, t->value, isGlobal);
    default:
        throw runtime_error("Unknown const type");
    }
}

void semantic::getArrayType(ArrayDesc& desc, int size)
{
    auto* newTy = ArrayType::get(desc.arrayType, size);
    auto* newConst =  ConstantArray::get(newTy, vector(size, desc.constant));
    desc.arrayType = newTy;
    desc.constant = newConst;
}

void semantic::getArrayType(token::TokenDesc* t, ArrayDesc& desc, int size)
{
    switch (t->token.token)
    {
    case token::TokenState::type_int:
        getArrayTypeInt(size, desc);
        break;
    case token::TokenState::type_real:
        getArrayTypeReal(size, desc);
        break;
    case token::TokenState::type_char:
        getArrayTypeChar(size, desc);
        break;
    case token::TokenState::type_bool:
        getArrayTypeBool(size, desc);
        break;
    default:
        throw runtime_error("Unknown array type");
    }
}

void semantic::regisVarArray(const token::TokenDesc* t, const ArrayDesc& desc, bool isGlobal)
{
    regisArray(t->value, false, desc, isGlobal);
}