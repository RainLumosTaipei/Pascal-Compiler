#pragma once

#include <llvm/IR/PassManager.h>

#include "llvm/Pass.h"

struct RmTerminatorPass : public llvm::FunctionPass {
    static char ID;
    explicit RmTerminatorPass() : llvm::FunctionPass(ID) {}
    bool runOnFunction(llvm::Function &F) override; 
};


class RemoveTerminatorPass : public llvm::PassInfoMixin<RemoveTerminatorPass>
{
public:
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM);
};