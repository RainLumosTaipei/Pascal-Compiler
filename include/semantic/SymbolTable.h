#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <llvm/IR/Value.h>

#include "lex/Lexer.h"


namespace semantic
{
    struct SymbolEntry;
    typedef std::unordered_map<std::string, SymbolEntry> ScopeTable;

    class SymbolTable {
    private:
        std::vector<ScopeTable> scopes;

    public:

        void enterScope();
        void leaveScope();
        void addVar(const std::string& name, llvm::Value* val, llvm::Type* type);
        bool findVar(token::TokenDesc* desc);
        bool findVar(const std::string& name);
        llvm::Value* randomVar();
        size_t deep();

        SymbolTable()
        {
            enterScope();
        }
    };

    SymbolTable& getSymbolTable();

}
