﻿#include "semantic/Ast.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/LegacyPassManager.h"

#include <iostream>
#include <llvm/IR/Verifier.h>

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

void ast::printIR()
{
    getModule().print(errs(), nullptr);
}

void ast::saveIR()
{
    std::error_code EC;
    raw_fd_ostream fileStream("output/output.ll", EC);
    if (EC)
    {
        errs() << "Error opening file: " << EC.message() << "\n";
        return;
    }
    getModule().print(fileStream, nullptr);
    fileStream.close();
}

void ast::saveASM()
{
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    string TargetTriple = sys::getDefaultTargetTriple();
    getModule().setTargetTriple(TargetTriple);

    std::string Error;
    auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);
    if (!Target) {
        errs() << Error;
        return;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto TheTargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, Reloc::PIC_);

    getModule().setDataLayout(TheTargetMachine->createDataLayout());

    if (verifyModule(getModule(), &errs())) {
        errs() << "Module verification failed\n";
        return ;
    }

    auto Filename = "output/output.o";
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);
    if (EC) {
        errs() << "Could not open file: " << EC.message();
        return ;
    }

    legacy::PassManager pass;
    auto FileType = CodeGenFileType::ObjectFile;
    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        errs() << "TheTargetMachine can't emit a file of this type";
        return ;
    }

    pass.run(getModule());
    dest.flush();
    outs() << "ASM has written to " << Filename << "\n";
}


Value* logError(const char* Str)
{
    cerr << Str << '\n';
    return nullptr;
}
