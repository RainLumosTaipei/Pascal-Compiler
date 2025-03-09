#include "semantic/VarDef.h"
#include "semantic/Ast.h"

#include <map>


#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

using namespace std;
using namespace llvm;
using namespace ast;
using namespace semantic;

inline map<string, Value*>& semantic::getIdTable()
{
    static map<string, Value*> idTable;
    return idTable;
}

namespace 
{
    void regisId(string id, Value* v)
    {
        getIdTable()[id] = v;
    }

    void regisGlobal(const std::string& id, bool isConst, Type* type, Constant* initialValue) {
        GlobalVariable *var = new GlobalVariable(
            getModule(), type, isConst, GlobalValue::ExternalLinkage, initialValue, id);
        regisId(id, var);
    }

    // 通用的注册变量函数模板
    template <typename T, unsigned BitWidth = 32, bool IsSigned = true>
    void regisVar(const std::string& id, bool isConstant) {
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

        regisGlobal(id, isConstant, type, zero);
    }

    // 变量注册函数
    void regisVarInt(const std::string& id) {
        regisVar<int>(id, false);
    }

    void regisVarReal(const std::string& id) {
        regisVar<float>(id, false);
    }

    void regisVarChar(const std::string& id) {
        regisVar<char, 8, false>(id, false);
    }

    void regisVarBool(const std::string& id) {
        regisVar<bool, 8, false>(id, false);
    }

    // 常量注册函数
    void regisConstInt(const std::string& id) {
        regisVar<int>(id, true);
    }

    void regisConstReal(const std::string& id) {
        regisVar<float>(id, true);
    }

    void regisConstChar(const std::string& id) {
        regisVar<char, 8, false>(id, true);
    }

    void regisConstBool(const std::string& id) {
        regisVar<bool, 8, false>(id, true);
    }
}

namespace
{
    template <typename T, unsigned BitWidth = 32, bool IsSigned = true>
    void regisVarWithValue(const std::string& id,  const string& val, bool isConstant) {
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

        regisGlobal(id, isConstant, type, zero);
    }

    void regisVarIntWithValue(const std::string& id, const string& val) {
        regisVarWithValue<int>(id, val, false);
    }

    void regisVarRealWithValue(const std::string& id, const string& val) {
        regisVarWithValue<float>(id, val, false);
    }

    void regisVarCharWithValue(const std::string& id, const string& val) {
        regisVarWithValue<char, 8, false>(id, val, false);
    }

    void regisVarBoolWithValue(const std::string& id, const string& val) {
        regisVarWithValue<bool, 8, false>(id, val, false);
    }

    // 常量注册函数
    void regisConstIntWithValue(const std::string& id, const string& val) {
        regisVarWithValue<int>(id, val, true);
    }

    void regisConstRealWithValue(const std::string& id, const string& val) {
        regisVarWithValue<float>(id, val, true);
    }

    void regisConstCharWithValue(const std::string& id, const string& val) {
        regisVarWithValue<char, 8, false>(id, val, true);
    }

    void regisConstBoolWithValue(const std::string& id, const string& val) {
        regisVarWithValue<bool, 8, false>(id, val, true);
    }
}

namespace
{
    void regisArray(const std::string& id, bool isConstant, Type* type, size_t size)
    {
        LLVMContext& context = getContext();
        ArrayType *ArrayTy = ArrayType::get(type, size);
        vector InitialValues(size, ConstantInt::get(type, 0));
        Constant *zero = ConstantArray::get(ArrayTy, InitialValues);
        
        regisGlobal(id, isConstant, type, zero);
    }
}

void semantic::regisVar(string id, token::TokenDesc& type)
{
    switch (type.token.token)
    {
    case token::TokenState::type_int:
        regisVarInt(id);
        break;
    case token::TokenState::type_real:
        regisVarReal(id);
        break;
    case token::TokenState::type_char:
        regisVarChar(id);
        break;
    case token::TokenState::type_bool:
        regisVarBool(id);
        break;
    default:
        throw runtime_error("Unknown var type");
    }
}

void semantic::regisConstWithValue(const string& id, token::TokenDesc& t)
{
    switch (t.token.token)
    {
    case token::TokenState::num:
        if(t.value.find('.') != string::npos)
            return regisConstRealWithValue(id, t.value);
        return regisConstIntWithValue(id, t.value);
    case token::TokenState::letter:
        return regisConstCharWithValue(id, t.value);
    default:
        throw runtime_error("Unknown const type");
    }
}

void semantic::regisArray(const std::string& id, token::TokenDesc& t, ArrayShape& shape){}