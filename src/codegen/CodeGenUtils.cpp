#include "codegen/CodeGenUtils.h"
#include <iostream> 
namespace pascal2c {
namespace codegen {

std::string CodeGenUtils::make_indent(int depth) {
    return std::string(depth * 4, ' '); // 每层缩进4空格
}

std::string CodeGenUtils::pascal_type_to_c(const std::string& pascal_type) {
    static const std::unordered_map<std::string, std::string> type_map = {
        {"integer", "int"},
        {"real",    "double"},
        {"char",    "char"},
        {"boolean", "bool"}
    };

    auto it = type_map.find(pascal_type);
    if (it != type_map.end()) {
        return it->second;
    } else {
        // 默认返回 int 并输出警告
        std::cerr << "[WARN] 未知的Pascal类型: " << pascal_type << ", 默认转换为 int" << std::endl;
        return "int";
    }
}

std::string CodeGenUtils::wrap_in_parentheses(const std::string& expr) {
    // 简化实现：所有表达式都加括号（后续可优化优先级判断）
    return "(" + expr + ")";
}

} // namespace codegen
} // namespace pascal2c