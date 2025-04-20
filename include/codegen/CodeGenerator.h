#pragma once
#include "codegen/SymbolTable.h"
#include "codegen/CodeGenUtils.h"
#include "syntax/Ast.h"
#include <string>

namespace pascal2c {
namespace codegen {

/**
 * @brief AST到C代码转换器核心类
 */
class CodeGenerator {
public:
    CodeGenerator();

    /**
     * @brief 生成完整的C代码字符串
     * @param root AST根节点
     */
    std::string generate(const syntax::Ast* root);

private:
    // 递归分发入口
    void generate_node(const syntax::Ast* node);

    // 节点类型处理函数
    void generate_program(const syntax::Ast* node);
    void generate_var_defs(const syntax::Ast* node);
    void generate_stmt_list(const syntax::Ast* node);
    void generate_assign_stmt(const syntax::Ast* node);
    void generate_write_stmt(const syntax::Ast* node);
    void generate_expression(const syntax::Ast* node, std::string& output);
    void generate_var_def(const syntax::Ast* node);        // 正确声明
    void generate_var_with_type(const syntax::Ast* node);  // 正确声明
    void generate_prog_body(const syntax::Ast* node);
    void generate_stmt_base(const syntax::Ast* node);
    void generate_main(const syntax::Ast* node);
    void generate_stmt(const syntax::Ast* node);
    void generate_begin(const syntax::Ast* node);
    void generate_exp_list(const syntax::Ast* node);
private:
    std::string _code;       // 生成的C代码
    SymbolTable _symbols;     // 符号表
    int _indent_depth = 0;   // 当前缩进层级
    std::string _current_expr;
};

} // namespace codegen
} // namespace pascal2c

