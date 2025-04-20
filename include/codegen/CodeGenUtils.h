#pragma once

#include <string>
#include <unordered_map>
#include <iostream> 
namespace pascal2c {
namespace codegen {

/**
 * @brief 代码生成工具函数集合
 */
class CodeGenUtils {
public:
    CodeGenUtils() = delete; // 禁用实例化

    /**
     * @brief 生成缩进字符串
     * @param depth 缩进层级（每层4空格）
     * @return 缩进字符串，如 depth=1 → "    "
     */
    static std::string make_indent(int depth);

    /**
     * @brief 将Pascal类型转换为C类型
     * @param pascal_type Pascal类型（如 "integer"）
     * @return C类型（如 "int"），未知类型返回空字符串
     */
    static std::string pascal_type_to_c(const std::string& pascal_type);

    /**
     * @brief 为表达式添加括号（根据优先级优化）
     * @param expr 原始表达式
     * @return 带括号的表达式（如 "a + (b * c)"）
     */
    static std::string wrap_in_parentheses(const std::string& expr);
};

} // namespace codegen
} // namespace pascal2c

