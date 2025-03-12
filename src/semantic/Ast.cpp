#include "semantic/Ast.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Verifier.h"

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

void ast::printCode()
{
    getModule().print(errs(), nullptr);
}

void ast::printCodeToFile()
{
    std::error_code EC;
    raw_fd_ostream fileStream("output.ll", EC);
    if (EC) {
        errs() << "Error opening file: " << EC.message() << "\n";
        return;
    }
    getModule().print(fileStream, nullptr);
    fileStream.close();
}


Value* logError(const char* Str) {
    cerr << Str << '\n';
    return nullptr;
}


//
// // 函数定义
// Function* ast::FunctionAST::codegen() {
//     // First, check for an existing function from a previous 'extern' declaration.
//     Function* TheFunction = module->getFunction(proto->getName());
//
//     if (!TheFunction)
//         TheFunction = proto->codegen();
//
//     if (!TheFunction)
//         return nullptr;
//
//     // Create a new basic block to start insertion into.
//     BasicBlock* BB = BasicBlock::Create(*context, "entry", TheFunction);
//     builder->SetInsertPoint(BB);
//
//     // Record the function arguments in the NamedValues map.
//     idTable.clear();
//     for (auto& Arg : TheFunction->args())
//         idTable[std::string(Arg.getName())] = &Arg;
//
//     if (Value* RetVal = body->codegen()) {
//         // Finish off the function.
//         builder->CreateRet(RetVal);
//
//         // Validate the generated code, checking for consistency.
//         verifyFunction(*TheFunction);
//
//         return TheFunction;
//     }
//
//     // Error reading body, remove function.
//     TheFunction->eraseFromParent();
//     return nullptr;
//}