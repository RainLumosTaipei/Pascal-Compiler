#include "codegen/CodeGenerator.h"
#include <sstream>

using namespace pascal2c::codegen;

CodeGenerator::CodeGenerator() {
    _code.reserve(4096); // 预分配内存
}

std::string CodeGenerator::generate(const syntax::Ast* root) {
    std::cout << "[DEBUG] 开始生成代码，根节点类型: " << root->t->str << std::endl;
    _code.clear();
    generate_node(root);
    std::cout << "[DEBUG] 生成的代码内容: \n" << _code << std::endl;
    return _code;
}

//=== 私有方法实现 ===//
// src/codegen/CodeGenerator.cpp
void CodeGenerator::generate_node(const syntax::Ast* node) {
    if (!node || !node->t) return;

    token::TokenState type = static_cast<token::TokenState>(node->t->token);
    std::cout << "[DEBUG] 处理节点类型: " << *(node->t) << std::endl;
    std::cout << "[DEBUG] 处理节点类型: " << static_cast<int>(type) 
    << " (" << token::tokenNames[type] << ")" << std::endl;

    switch (type) {
        case token::TokenState::prog:         // 枚举值=0
            generate_program(node);
            break;
        case token::TokenState::prog_body:    // 枚举值=2
            generate_prog_body(node);
            break;
        case token::TokenState::var_defs:     // 枚举值=13
            generate_var_defs(node);
            break;
        case token::TokenState::var_def:      // 枚举值=14
            generate_var_def(node);
            break;
        case token::TokenState::var_with_type: // 枚举值=15
            generate_var_with_type(node);
            break;
        case token::TokenState::stmt_base:    // 枚举值=23
        std::cout << "进入 generate_stmt_base" << std::endl;
            generate_stmt_base(node);
            break;
        case token::TokenState::stmt:       // TokenState=29
            std::cout << "进入 generate_stmt" << std::endl;
            generate_stmt(node);
            break;
        case token::TokenState::op_assign:    // 枚举值=48
            generate_assign_stmt(node);
            break;
        case token::TokenState::key_write:    // 枚举值=69
            generate_write_stmt(node);
            break;
        case token::TokenState::stmt_list:   // 新增：处理语句列表
            generate_stmt_list(node);
            break;
        case token::TokenState::begin:       // 新增：处理begin块
            generate_begin(node);
            break;
        case token::TokenState::main:  // 新增：处理 main 节点
            std::cout << "进入 generate_main" << std::endl;
            generate_main(node);
            break;
        case token::TokenState::exp: // 处理表达式节点
            std::cout << "进入 generate_exp" << std::endl;
            generate_expression(node, _current_expr);
            break;
        case token::TokenState::exp_list:  // Type=32
            std::cout << "进入 generate_exp_list" << std::endl;
            generate_exp_list(node);
            break;
        default:
            std::cout << "[WARN] 未处理的节点类型: " << *(node->t) << std::endl;
    }
}
void CodeGenerator::generate_stmt(const syntax::Ast* node) {
    // stmt 的子节点是 stmt_base
    for (const auto& child : node->child) {
        generate_node(child);
    }
}
void CodeGenerator::generate_begin(const syntax::Ast* node) {
    // begin 本身不需要生成代码，但需要遍历其子节点（如stmt_list）
    for (const auto& child : node->child) {
        generate_node(child);
    }
}
void CodeGenerator::generate_stmt_base(const syntax::Ast* node) {
    std::cout << "进入 generate_stmt_base" << std::endl;
    for (const auto& child : node->child) {
        generate_node(child); // 递归处理子节点（如 := 或 write）
    }
}
void CodeGenerator::generate_main(const syntax::Ast* node) {
    std::cout << "处理 main 节点，遍历其子节点" << std::endl;
    for (const auto& child : node->child) {
        // 确保处理 begin 和 stmt_list
        generate_node(child);
    }
}
//=== 具体生成逻辑 ===//
// src/codegen/CodeGenerator.cpp
void CodeGenerator::generate_stmt_list(const syntax::Ast* node) {
    std::cout << "[DEBUG] === 处理stmt_list ===" << std::endl;
    for (const auto& child : node->child) {
        token::TokenState child_type = static_cast<token::TokenState>(child->t->token);
        if (child_type == token::TokenState::p_semicolon) {
            std::cout << "[DEBUG] 跳过分号 ;" << std::endl;
            continue;
        }
        std::cout << "[DEBUG] 处理stmt_list子节点: " 
                  << token::tokenNames[child_type] << std::endl;
        generate_node(child);
    }
}

void CodeGenerator::generate_program(const syntax::Ast* node) {
    std::cout << "[DEBUG] 生成程序框架（main函数）" << std::endl;
    _code += "int main() {\n";
    _indent_depth++;

    std::cout << "[DEBUG] 程序节点子节点数量: " << node->child.size() << std::endl;
    for (size_t i = 0; i < node->child.size(); ++i) {
        const auto& child = node->child[i];

        generate_node(child);
    }

    _code += CodeGenUtils::make_indent(_indent_depth) + "return 0;\n";
    _indent_depth--;
    _code += "}\n";
    std::cout << "[DEBUG] 程序框架生成完毕" << std::endl;
}


void CodeGenerator::generate_assign_stmt(const syntax::Ast* node) {
    std::string left, right;
    std::cout << "[DEBUG] 处理赋值语句" << std::endl;

    // 遍历所有子节点识别左值和右值
    for (const auto& child : node->child) {
        token::TokenState child_type = static_cast<token::TokenState>(child->t->token);
        if (child_type == token::TokenState::var) {
            generate_expression(child, left);
        } else if (child_type == token::TokenState::exp) {
            generate_expression(child, right);
        }
    }

    if (!left.empty() && !right.empty()) {
        _code += CodeGenUtils::make_indent(_indent_depth) 
               + left + " = " + right + ";\n";
        std::cout << "[DEBUG] 生成赋值语句: " << left << " = " << right << ";" << std::endl;
    } else {
        std::cerr << "[ERROR] 赋值语句缺少左值或右值" << std::endl;
    }
}
void CodeGenerator::generate_exp_list(const syntax::Ast* node) {
    std::string args;
    for (const auto& child : node->child) {
        std::string expr;
        generate_expression(child, expr);
        if (!args.empty()) args += ", ";
        args += expr;
    }
    _current_expr = args; // 存储参数
}

void CodeGenerator::generate_write_stmt(const syntax::Ast* node) {
    _current_expr.clear();
    for (const auto& child : node->child) {
        if (static_cast<token::TokenState>(child->t->token) == token::TokenState::exp_list) {
            generate_exp_list(child);
            break;
        }
    }

    if (!_current_expr.empty()) {
        _code += CodeGenUtils::make_indent(_indent_depth)
               + "printf(\"%d\\n\", " + _current_expr + ");\n";
        std::cout << "[DEBUG] 生成write语句: printf(\"%d\\n\", " << _current_expr << ")" << std::endl;
    } else {
        std::cerr << "[ERROR] write语句缺少参数" << std::endl;
    }
}
void CodeGenerator::generate_expression(const syntax::Ast* node, std::string& output) {
    if (!node || !node->t) return;

    token::TokenState type = static_cast<token::TokenState>(node->t->token);
    std::cout << "[DEBUG] 处理表达式节点: " << node->t->str << " (Type=" << static_cast<int>(type) << ")" << std::endl;

    // 处理变量（支持 var → factor → id 结构）
    if (type == token::TokenState::var) {
        for (const auto& child : node->child) {
            generate_expression(child, output); // 递归处理子节点
            if (!output.empty()) break;
        }
    } 
    // 处理数值
    else if (type == token::TokenState::num) {
        output = node->t->str;
        std::cout << "[DEBUG] 提取数值: " << output << std::endl;
    } 
    // 处理 ID（直接变量名）
    else if (type == token::TokenState::id) {
        output = node->t->str;
        std::cout << "[DEBUG] 提取变量名: " << output << std::endl;
    }
}
void CodeGenerator::generate_var_defs(const syntax::Ast* node) {
    for (const auto& child : node->child) {
        // 跳过非 var_def 节点（如 ; 和 var）
        if (static_cast<token::TokenState>(child->t->token) != token::TokenState::var_def) {
            continue;
        }
        generate_var_def(child);
    }
}

void CodeGenerator::generate_var_def(const syntax::Ast* node) {
    for (const auto& child : node->child) {
        // 检查子节点是否为 var_with_type (TokenState=15)
        if (static_cast<token::TokenState>(child->t->token) == token::TokenState::var_with_type) {
            generate_var_with_type(child);
        }
    }
}


void CodeGenerator::generate_var_with_type(const syntax::Ast* node) {
    std::string var_name, pascal_type;

    // 提取变量名（第三个子节点）
    if (node->child.size() > 2) {
        const syntax::Ast* id_node = node->child[2];
        if (static_cast<token::TokenState>(id_node->t->token) == token::TokenState::id) {
            var_name = id_node->t->str;
            std::cout << "[DEBUG] 提取变量名: " << var_name << std::endl;
        }
    }

    // 提取类型（第一个子节点是 type）
    if (node->child.size() > 0) {
        const syntax::Ast* type_node = node->child[0];
        if (static_cast<token::TokenState>(type_node->t->token) == token::TokenState::type) {
            // 遍历 type 的子节点，找到 type_base
            for (const auto& type_child : type_node->child) {
                if (static_cast<token::TokenState>(type_child->t->token) == token::TokenState::type_base) {
                    // 提取 type_base 的实际类型（如 integer）
                    for (const auto& type_base_child : type_child->child) {
                        token::TokenState type_base_type = static_cast<token::TokenState>(type_base_child->t->token);
                        if (type_base_type == token::TokenState::type_int) {
                            pascal_type = "integer"; // 或从 t->str 直接读取
                        } else if (type_base_type == token::TokenState::type_real) {
                            pascal_type = "real";
                        }
                        // 其他类型处理...
                    }
                    std::cout << "[DEBUG] 提取Pascal类型: " << pascal_type << std::endl;
                }
            }
        }
    }

    // 转换为C类型
    std::string c_type = CodeGenUtils::pascal_type_to_c(pascal_type);
    _symbols.add_variable(var_name, c_type);
    std::cout << "[DEBUG] 生成变量声明: " << c_type << " " << var_name << ";\n";

    _code += CodeGenUtils::make_indent(_indent_depth) 
           + c_type + " " + var_name + ";\n";
}

void CodeGenerator::generate_prog_body(const syntax::Ast* node) {
    std::cout << "进入generate_prog_body" <<std::endl;
    
    // 先处理所有变量声明
    for (const auto& child : node->child) {
        token::TokenState type = static_cast<token::TokenState>(child->t->token);
        if (type == token::TokenState::var_defs) {
            generate_var_defs(child);
        }
    }
    
    // 再处理其他节点（如 stmt_list）
    for (const auto& child : node->child) {
        token::TokenState type = static_cast<token::TokenState>(child->t->token);
        if (type != token::TokenState::var_defs) {
            generate_node(child);
        }
    }
}