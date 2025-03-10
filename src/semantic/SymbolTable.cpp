#include "semantic/SymbolTable.h"

using namespace std;
using namespace semantic;

SymbolTable& semantic::getSymbolTable()
{
    static SymbolTable symbolTable;
    return symbolTable;
}

void semantic::SymbolTable::enterScope() {
    scopes.emplace_back();
}


void semantic::SymbolTable::leaveScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}


void semantic::SymbolTable::addVar(const std::string& name, llvm::Value* val) {
    if (!scopes.empty()) {
        scopes.back()[name] = {val};
    }
}


bool semantic::SymbolTable::findVar(const std::string& name, SymbolEntry& entry) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            entry = found->second;
            return true;
        }
    }
    return false;
}

llvm::Value* semantic::SymbolTable::randomVar()
{
    if (!scopes.empty())
    {
        auto it = scopes.rbegin()->begin();
        return it->second.val;
    }
    return nullptr;
}