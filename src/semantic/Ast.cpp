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
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"


#include <iostream>
#include <llvm/IR/Verifier.h>

#include "semantic/SymbolTable.h"

using namespace llvm;
using namespace std;

LLVMContext& ast::getContext()
{
    static LLVMContext context;
    return context;
}

Module& ast::getModule()
{
    return *getModules().back();
}

vector<unique_ptr<Module>>& ast::getModules()
{
    static vector<unique_ptr<Module>> modules;
    return modules;
}

IRBuilder<>& ast::getBuilder()
{
    static IRBuilder<> builder(getContext());
    return builder;
}

Linker& ast::getLinker()
{
    static Linker linker(getModule());
    return linker;
}

void ast::resetModule()
{
    unique_ptr<Module> module = make_unique<Module>("Pascal Compiler", getContext());
    getModules().push_back(move(module));
    semantic::getSymbolTable().clear();
    
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

int ast::saveIR(string filename)
{
    getModule().setSourceFileName(filename);
    std::error_code EC;
    std::string fullPath = filename;
    size_t dotPos = fullPath.find_last_of('.');
    if (dotPos != std::string::npos) {
        fullPath = fullPath.substr(0, dotPos) + ".ll";
    } else {
        fullPath += ".ll";
    }
    raw_fd_ostream fileStream(fullPath, EC, sys::fs::CD_CreateAlways );
    if (EC)
    {
        errs() << "Error opening file: " << EC.message() << "\n";
        return 1;
    }
    getModule().print(fileStream, nullptr);
    fileStream.close();
    return 0;
}

int ast::saveASM(string filename)
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
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto TheTargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, Reloc::PIC_);

    getModule().setDataLayout(TheTargetMachine->createDataLayout());

    if (verifyModule(getModule(), &errs())) {
        errs() << "Module verification failed\n";
        return 1;
    }

    std::string fullPath = filename;
    size_t dotPos = fullPath.find_last_of('.');
    if (dotPos != std::string::npos) {
        fullPath = fullPath.substr(0, dotPos) + ".o";
    } else {
        fullPath += ".o";
    }
    std::error_code EC;
    raw_fd_ostream dest(fullPath, EC, sys::fs::CD_CreateAlways);
    if (EC) {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }

    legacy::PassManager pass;
    auto FileType = CodeGenFileType::ObjectFile;
    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }

    pass.run(getModule());
    dest.flush();
    return 0;
}

int ast::link()
{
    auto it = ++getModules().rbegin();
    for(; it!=getModules().rend(); ++it)
    {
        if(getLinker().linkInModule(move(*it)))
            return 1;
    }
    return 0;
}

int ast::execute()
{
    // ExecutionEngine *EE = EngineBuilder(std::move(getModule()))
    //                       .create();
    //
    // if (!EE) {
    //     return 1;
    // }
    //
    // // 运行主函数（假设存在）
    // Function *Main = EE->FindFunctionNamed("main");
    // if (Main) {
    //     std::vector<GenericValue> NoArgs;
    //     EE->runFunction(Main, NoArgs);
    // }

    return 0;
}


Value* logError(const char* Str)
{
    cerr << Str << '\n';
    return nullptr;
}
