#pragma once

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"

namespace ast
{
    llvm::LLVMContext& getContext();
    llvm::Module& getModule();
    llvm::IRBuilder<>& getBuilder();

    void initPass();
    llvm::FunctionPassManager& getFPM();
    llvm::FunctionAnalysisManager& getFAM();

    void printIR();
    int saveIR(char* filename);
    int saveASM(char* filename);
}
