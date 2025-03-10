#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <llvm/IR/Value.h>


namespace semantic
{
    struct SymbolEntry {
        llvm::Value* val;
    };

    typedef std::unordered_map<std::string, SymbolEntry> ScopeTable;

    class SymbolTable {
    private:
        std::vector<ScopeTable> scopes;

    public:

        void enterScope();
        void leaveScope();
        void addVar(const std::string& name, llvm::Value* val);
        bool findVar(const std::string& name, SymbolEntry& entry);
        llvm::Value* randomVar();

        SymbolTable()
        {
            enterScope();
        }
    };

    SymbolTable& getSymbolTable();

}
