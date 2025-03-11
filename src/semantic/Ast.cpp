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


Value* logError(const char* Str) {
    cerr << Str << '\n';
    return nullptr;
}




//
// // id 变量
// Value* ast::VariableExprAST::codegen() {
//     Value* V = idTable[name];
//     if (!V)
//         return logError("Unknown variable name");
//     return V;
// }
//

//
// // 函数调用
// Value* ast::CallExprAST::codegen() {
//     Function* CalleeF = module->getFunction(Callee);
//     if (!CalleeF)
//         return logError("Unknown function referenced");
//
//
//     if (CalleeF->arg_size() != Args.size())
//         return logError("Incorrect # arguments passed");
//
//     vector<Value*> ArgsV;
//     for (unsigned i = 0, e = Args.size(); i != e; ++i) {
//         ArgsV.push_back(Args[i]->codegen());
//         if (!ArgsV.back())
//             return nullptr;
//     }
//     return builder->CreateCall(CalleeF, ArgsV, "call");
// }
//

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