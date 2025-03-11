#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <llvm/IR/Value.h>


namespace semantic
{
    struct SymbolEntry {
        // 保存变量的地址，而不是右值
        llvm::Value* val;
        llvm::Type* type;
    };

    typedef std::unordered_map<std::string, SymbolEntry> ScopeTable;

    class SymbolTable {
    private:
        std::vector<ScopeTable> scopes;

    public:

        void enterScope();
        void leaveScope();
        void addVar(const std::string& name, llvm::Value* val, llvm::Type* type);
        bool findVar(const std::string& name, SymbolEntry& entry);
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
