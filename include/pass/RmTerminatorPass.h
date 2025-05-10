#pragma once

#include <llvm/IR/PassManager.h>

#include "llvm/Pass.h"

struct RmTerminatorPass : llvm::FunctionPass
{
    static char ID;

    explicit RmTerminatorPass() : FunctionPass(ID)
    {
    }

    bool runOnFunction(llvm::Function& F) override;
};


class RemoveTerminatorPass : public llvm::PassInfoMixin<RemoveTerminatorPass>
{
public:
    llvm::PreservedAnalyses run(llvm::Function& F, llvm::FunctionAnalysisManager& FAM);
};
