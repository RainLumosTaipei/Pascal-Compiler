#include "pass/RmTerminatorPass.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"

using namespace llvm;


bool RmTerminatorPass::runOnFunction(Function &F) {
    bool Changed = false;
    for (auto &BB : F) {
        bool HasTerminator = false;
        for (auto I = BB.begin(); I != BB.end(); ++I) {
            if (I->isTerminator()) {
                if (HasTerminator) {
                    I->eraseFromParent();
                    Changed = true;
                } else {
                    HasTerminator = true;
                }
            }
        }
    }
    return Changed;
}

char RmTerminatorPass::ID = 0;
static RegisterPass<RmTerminatorPass>
    X("remove-extra-terminators", "Remove extra terminators from basic blocks");


PreservedAnalyses RemoveTerminatorPass::run(Function &F, FunctionAnalysisManager &FAM) {
    bool Changed = false;
    std::vector<Instruction*> RemovedInsts;
    for (auto &BB : F) {
        bool HasTerminator = false;
        for (auto& I : BB) {
            if (I.isTerminator()) {
                if (HasTerminator) {
                    RemovedInsts.push_back(&I);
                    Changed = true;
                } else {
                    HasTerminator = true;
                }
            }
        }
    }
    if (Changed)
        for (auto &I : RemovedInsts)
            I->eraseFromParent();
    
    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}
