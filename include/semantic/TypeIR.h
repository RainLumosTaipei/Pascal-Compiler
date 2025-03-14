#pragma once

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

#include "lex/Lexer.h"
#include "semantic/Ast.h"

namespace semantic
{
    inline llvm::Type* intTy = llvm::IntegerType::get(ast::getContext(), 32);
    inline llvm::Type* charTy = llvm::IntegerType::get(ast::getContext(), 8);
    inline llvm::Type* boolTy = llvm::IntegerType::get(ast::getContext(), 1);
    inline llvm::Type* realTy = llvm::Type::getFloatTy(ast::getContext());

    inline llvm::Constant* trueBool = llvm::ConstantInt::get(boolTy, 1, false);
    inline llvm::Constant* falseBool = llvm::ConstantInt::get(boolTy, 0, false);
    inline llvm::Constant* zeroInt = llvm::ConstantInt::get(intTy, 0, true);
    inline llvm::Constant* zeroChar = llvm::ConstantInt::get(charTy, 0, true);
    inline llvm::Constant* zeroReal = llvm::ConstantFP::get(realTy, 0);

    void typeInt(token::TokenDesc* type);
    void typeChar(token::TokenDesc* type);
    void typeBool(token::TokenDesc* type);
    void typeReal(token::TokenDesc* type);
    void typeArray(token::TokenDesc* type, int size);

    llvm::Constant* toInt(const std::string& str);
    llvm::Constant* toChar(const std::string& str);
    llvm::Constant* toBool(const std::string& str);
    llvm::Constant* toReal(const std::string& str);
}
