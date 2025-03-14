#pragma once
#include <stack>
#include <vector>
#include <llvm/IR/Function.h>


#include "lex/Lexer.h"
#include "lex/Token.h"

namespace semantic
{
    using FuncDesc = struct
    {
        std::vector<token::TokenDesc*> paraType;
        std::vector<token::TokenDesc*> paraName;
        token::TokenDesc* rev;
        token::TokenDesc* name;
        bool isVoid;
    };

    using BlockType = enum
    {
        iff,
        whl,
        fore
    };

    using BlockDesc = struct BlockDesc
    {
        llvm::BasicBlock* entry;
        llvm::BasicBlock* then;
        llvm::BasicBlock* els;
        llvm::BasicBlock* merge;

        BlockType type;

        BlockDesc(BlockType type);
        void condBr(token::TokenDesc* desc);
        void condBr(token::TokenDesc*, token::TokenDesc*);
        void thenBr();
        void elsBr();
        void elsBr(token::TokenDesc* desc);
    };

    void regisFunc(const FuncDesc& desc);
    void retFunc(token::TokenDesc* ret);
    void retFunc();
    void callFunc(token::TokenDesc* idf, const std::vector<token::TokenDesc*>& exps);
    void startFuncBlock(const std::string& name);
    void endFuncBlock();
    void callRead(const std::vector<token::TokenDesc*>& vars);
    void callWrite(const std::vector<token::TokenDesc*>& exps);
}
