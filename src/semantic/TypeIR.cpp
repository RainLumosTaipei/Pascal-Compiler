#include "semantic/TypeIR.h"

using namespace std;
using namespace llvm;

void semantic::typeInt(token::TokenDesc* type)
{
    type->entry.type = intTy;
    type->entry.val = zeroInt;
}

void semantic::typeChar(token::TokenDesc* type)
{
    type->entry.type = charTy;
    type->entry.val = zeroChar;
}

void semantic::typeBool(token::TokenDesc* type)
{
    type->entry.type = boolTy;
    type->entry.val = falseBool;
}

void semantic::typeReal(token::TokenDesc* type)
{
    type->entry.type = realTy;
    type->entry.val = zeroReal;
}

void semantic::typeArray(token::TokenDesc* type, int size)
{
    auto* newTy = ArrayType::get(type->entry.type, size);
    auto contValue = cast<Constant>(type->entry.val);
    auto* newConst = ConstantArray::get(newTy, vector(size, contValue));
    type->entry.type = newTy;
    type->entry.val = newConst;
}

llvm::Constant* semantic::toInt(const std::string& str)
{
    return llvm::ConstantInt::get(intTy, stoi(str), true);
}

llvm::Constant* semantic::toChar(const std::string& str)
{
    return llvm::ConstantInt::get(charTy, static_cast<int>(str[0]), true);
}

llvm::Constant* semantic::toBool(const std::string& str)
{
    if (str == "true" || str == "True" || str == "TRUE")
        return trueBool;
    return falseBool;
}

llvm::Constant* semantic::toReal(const std::string& str)
{
    return ConstantFP::get(realTy, stof(str));
}
