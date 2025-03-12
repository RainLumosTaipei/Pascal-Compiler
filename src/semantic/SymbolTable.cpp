#include "semantic/SymbolTable.h"

#include "semantic/Exp.h"

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

size_t semantic::SymbolTable::deep()
{
    return scopes.size();
}


void semantic::SymbolTable::addVar(const std::string& name, llvm::Value* val, llvm::Type* type) {
    if (!scopes.empty()) {
        scopes.back()[name] = {val, type};
    }
}


bool semantic::SymbolTable::findVar(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return true;
        }
    }
    return false;
}


bool semantic::SymbolTable::findVar(token::TokenDesc* desc) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(desc->str);
        if (found != it->end()) {
            desc->entry = found->second;
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