#include "semantic/Ast.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

#include <iostream>

using namespace llvm;
using namespace std;

LLVMContext& ast::getContext()
{
    static LLVMContext context;
    return context;
}

Module& ast::getModule()
{
    static Module module("Pascal IR", getContext());
    return module;
}

IRBuilder<>& ast::getBuilder()
{
    static IRBuilder<> builder(getContext());
    return builder;
}

FunctionPassManager& ast::getFPM()
{
    static auto TheFPM = FunctionPassManager();
    return TheFPM;
}

FunctionAnalysisManager& ast::getFAM()
{
    static auto TheFAM = FunctionAnalysisManager();
    return TheFAM;
}

void ast::initPass()
{
    static auto TheLAM = LoopAnalysisManager();
    static auto TheCGAM = CGSCCAnalysisManager();
    static auto TheMAM = ModuleAnalysisManager();
    static auto ThePIC = PassInstrumentationCallbacks();
    static auto TheSI = StandardInstrumentations(ast::getContext(), true);

    TheSI.registerCallbacks(ThePIC, &TheMAM);
    getFPM().addPass(InstCombinePass());
    getFPM().addPass(ReassociatePass());
    getFPM().addPass(GVNPass());
    getFPM().addPass(SimplifyCFGPass());

    static PassBuilder PB;
    PB.registerModuleAnalyses(TheMAM);
    PB.registerFunctionAnalyses(getFAM());
    PB.crossRegisterProxies(TheLAM, getFAM(), TheCGAM, TheMAM);
}

void ast::printCode()
{
    getModule().print(errs(), nullptr);
}

void ast::printCodeToFile()
{
    std::error_code EC;
    raw_fd_ostream fileStream("output.ll", EC);
    if (EC)
    {
        errs() << "Error opening file: " << EC.message() << "\n";
        return;
    }
    getModule().print(fileStream, nullptr);
    fileStream.close();
}


Value* logError(const char* Str)
{
    cerr << Str << '\n';
    return nullptr;
}
