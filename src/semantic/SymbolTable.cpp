#include "semantic/SymbolTable.h"

#include "semantic/ExpIR.h"
#include "semantic/SemanticErr.h"

using namespace std;
using namespace semantic;

SymbolTable& semantic::getSymbolTable()
{
    static SymbolTable symbolTable;
    return symbolTable;
}

void SymbolTable::enterScope()
{
    scopes.emplace_back();
}

void SymbolTable::clear()
{
    scopes.clear();
    enterScope();
}

void SymbolTable::leaveScope()
{
    if (!scopes.empty())
    {
        scopes.pop_back();
    }
}

size_t SymbolTable::deep()
{
    return scopes.size();
}


void SymbolTable::addVar(const std::string& name, llvm::Value* val, llvm::Type* type)
{
    if (!scopes.empty())
    {
        scopes.back()[name] = {val, type};
    }
}


bool SymbolTable::findVar(const std::string& name)
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        auto found = it->find(name);
        if (found != it->end())
        {
            return true;
        }
    }
    throw runtime_error("variable can't be found");
}


bool SymbolTable::findVar(token::TokenDesc* desc)
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        auto found = it->find(desc->str);
        if (found != it->end())
        {
            desc->entry = found->second;
            return true;
        }
    }
    throw SemanticErr("variable can't be found", desc);
}

llvm::Value* SymbolTable::randomVar()
{
    if (!scopes.empty())
    {
        auto it = scopes.rbegin()->begin();
        return it->second.val;
    }
    return nullptr;
}
