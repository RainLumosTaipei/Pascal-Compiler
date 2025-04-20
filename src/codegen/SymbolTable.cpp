#include "codegen/SymbolTable.h"

namespace pascal2c {
namespace codegen {

SymbolTable::SymbolTable() {
    // 默认进入全局作用域
    enter_scope();
}

void SymbolTable::enter_scope() {
    _scopes.emplace_back(); // 压入新的空哈希表
}

void SymbolTable::exit_scope() {
    if (_scopes.empty()) 
        return; // 避免异常，实际使用时需结合业务逻辑处理
    
    _scopes.pop_back();
}

void SymbolTable::add_variable(const std::string& name, const std::string& type) {
    if (_scopes.empty()) 
        return; // 防御性编程
    
    _scopes.back()[name] = type;
}

std::string SymbolTable::get_variable_type(const std::string& name) const {
    // 从当前作用域向外逐层查找
    for (auto it = _scopes.rbegin(); it != _scopes.rend(); ++it) {
        const auto& scope = *it;
        if (scope.count(name))
            return scope.at(name);
    }
    return ""; // 未找到
}

} // namespace codegen
} // namespace pascal2c