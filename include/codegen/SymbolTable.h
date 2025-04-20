#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace pascal2c {
namespace codegen {

/**
 * @brief 符号表管理变量类型和作用域
 * 
 * 通过栈结构管理嵌套作用域，支持变量类型查询和添加
 */
class SymbolTable {
public:
    SymbolTable();

    /**
     * @brief 进入新作用域（如遇到 begin 块）
     */
    void enter_scope();

    /**
     * @brief 退出当前作用域（如遇到 end 块）
     */
    void exit_scope();

    /**
     * @brief 添加变量到当前作用域
     * @param name 变量名
     * @param type Pascal 类型（如 "integer"）
     */
    void add_variable(const std::string& name, const std::string& type);

    /**
     * @brief 查询变量类型（从内到外逐层查找）
     * @return C 类型（如 "int"），未找到返回空字符串
     */
    std::string get_variable_type(const std::string& name) const;

private:
    // 作用域栈，每个元素是一个作用域的符号表
    std::vector<std::unordered_map<std::string, std::string>> _scopes;
};

} // namespace codegen
} // namespace pascal2c

