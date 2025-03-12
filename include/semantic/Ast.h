#pragma once

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"


namespace ast
{
    llvm::LLVMContext& getContext();

    llvm::Module& getModule();

    llvm::IRBuilder<>& getBuilder();

    void printCode();
    void printCodeToFile();

}
