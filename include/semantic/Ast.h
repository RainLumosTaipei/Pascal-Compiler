#pragma once

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include "llvm/IR/IRBuilder.h"
#include "llvm/Linker/Linker.h"

namespace ast
{
    llvm::LLVMContext& getContext();
    llvm::Module& getModule();
    std::vector<std::unique_ptr<llvm::Module>>& getModules();
    llvm::IRBuilder<>& getBuilder();
    llvm::Linker& getLinker();

    void initPass();
    llvm::FunctionPassManager& getFPM();
    llvm::FunctionAnalysisManager& getFAM();
    void resetModule();

    void printIR();
    int saveIR(std::string filename);
    int saveASM(std::string filename);
    int link();
    int execute();
}
