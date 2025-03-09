#pragma once

#include "lex/Token.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

namespace ast
{

    // base expression
    class ExprAST {
    public:
        virtual ~ExprAST() = default;

        virtual llvm::Value* codegen() = 0;
    };

    // int 默认为32位 有符号
    class IntExprAST : public ExprAST {
    private:
        int v;

    public:
        IntExprAST(int Val) : v(Val) {}

        llvm::Value* codegen() override;
    };
    
    // real 默认为double 
    class RealExprAST : public ExprAST {
    private:
        double v;

    public:
        RealExprAST(double Val) : v(Val) {}

        llvm::Value* codegen() override;
    };

    // char 默认为8位 无符号
    class CharExprAST : public ExprAST {
    private:
        char v;

    public:
        CharExprAST(char Val) : v(Val) {}

        llvm::Value* codegen() override;
    };

    // bool 默认为1位 无符号
    class BoolExprAST : public ExprAST {
    private:
        bool v;

    public:
        BoolExprAST(bool Val) : v(Val) {}

        llvm::Value* codegen() override;
    };

    // id
    class VariableExprAST : public ExprAST {
    private:
        std::string name;

    public:
        VariableExprAST(const std::string& Name) : name(Name) {}

        llvm::Value* codegen() override;
    };

    // binary operator
    class BinaryOpExprAST : public ExprAST {
    private:
        token::TokenState op;
        std::unique_ptr<ExprAST> l, r;

    public:
        BinaryOpExprAST(token::TokenState Op, std::unique_ptr<ExprAST> l,
            std::unique_ptr<ExprAST> r)
            : op(Op), l(std::move(l)), r(std::move(r)) {}

        llvm::Value* codegen() override;
    };

    // unary operator
    class UnaryOpExprAST : public ExprAST {
    private:
        token::TokenState op;
        std::unique_ptr<ExprAST> v;

    public:
        UnaryOpExprAST(token::TokenState Op, std::unique_ptr<ExprAST> v)
            : op(Op), v(std::move(v)) {}

        llvm::Value* codegen() override;
    };

    // function call
    class CallExprAST : public ExprAST {
    private:
        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(const std::string& Callee,
            std::vector<std::unique_ptr<ExprAST>> Args)
            : Callee(Callee), Args(std::move(Args)) {}

        llvm::Value* codegen() override;
    };

    // function declare
    class PrototypeAST {
    private:
        std::string name;
        std::vector<std::string> args;

    public:
        PrototypeAST(const std::string& Name, std::vector<std::string> Args)
            : name(Name), args(std::move(Args)) {}

        llvm::Function* codegen();
        const std::string& getName() const { return name; }
    };

    // function def
    class FunctionAST {
    private:
        std::unique_ptr<PrototypeAST> proto;
        std::unique_ptr<ExprAST> body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto,
            std::unique_ptr<ExprAST> Body)
            : proto(std::move(Proto)), body(std::move(Body)) {}

        llvm::Function* codegen();
    };

    llvm::LLVMContext& getContext();

    llvm::Module& getModule();

    llvm::IRBuilder<>& getBuilder();

    void printCode();

}
