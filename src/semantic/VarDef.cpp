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
        getIdTable[id] = v;
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

void semantic::regisVar(string id, token::TokenState type)
{
    switch (type)
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
        throw runtime_error("Unknown token type");
    }
}