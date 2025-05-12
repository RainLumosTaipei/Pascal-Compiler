#ifndef PASCC_AST_H
#define PASCC_AST_H

#include <iostream>
#include <stack>
#include <vector>

#include "pstdlib.h"
#include "symbol_table.h"
namespace pascals {
namespace ast {

// 标准输出宏定义
#define PRINT(format, ...) fprintf(dst, format, ##__VA_ARGS__);

// 抽象语法树节点的基类
class Node {
 public:
  // 构造函数，初始化父节点为空
  Node() : parent_(nullptr){};
  // 虚析构函数，用于正确释放子节点资源
  virtual ~Node() {
    for (auto child : child_list_) {
      delete child; // 删除子节点列表中的每个子节点
    }
  };

  // 静态类型转换
  template <typename T>
  T *StaticCast() {
    return dynamic_cast<T *>(this); // 使用 dynamic_cast 进行向下类型转换
  }
  // 动态类型转换 (与StaticCast在此处实现相同)
  template <typename T>
  T *DynamicCast() {
    return dynamic_cast<T *>(this); // 使用 dynamic_cast 进行向下类型转换
  }

  // 设置父节点
  void set_parent(Node *parent) { parent_ = parent; }
  // 获取父节点
  Node *parent() { return parent_; }
  // 添加子节点到列表末尾
  void append_child(Node *child) { child_list_.emplace_back(child); }
  // 获取指定位置的子节点，支持负数索引 (从后往前)
  Node *get(int pos) {
    return child_list_[pos < 0 ? pos += child_list_.size() : pos];
  }

  // 格式化输出指定位置的子节点到文件流
  void FormatAt(int pos, FILE *dst) {
    return child_list_[pos < 0 ? pos + child_list_.size() : pos]->Format(dst);
  }
  // 虚函数，递归格式化输出当前节点及其所有子节点到文件流
  virtual void Format(FILE *dst) {
    for (auto child : child_list_) child->Format(dst); // 遍历并格式化所有子节点
  }

 protected:
  Node *parent_; // 指向父节点的指针
  std::vector<Node *> child_list_; // 存储子节点的动态数组
};

// 抽象语法树 (AST) 类
class AST {
 public:
  // 析构函数，释放AST资源
  ~AST() {
    log_debug("~AST(): delete ast"); // 记录析构日志
    if (root_ != nullptr) delete root_; // 如果根节点存在，则删除
  }
  // 检查AST是否有效 (有效标志为true且根节点存在)
  bool Valid() { return valid_ && root_ != nullptr; }
  // 设置AST的有效性标志
  void set_valid(bool v) { valid_ = v;}
  // 获取根节点
  Node *root() { return root_; }
  // 获取Pascal标准库对象指针
  PStdLibs *libs() { return &libs_; }
  // 设置根节点
  void set_root(Node *root) { root_ = root; }

  // 格式化输出整个AST到文件流
  void Format(FILE *dst);

 private:
  bool valid_ = false; // AST是否有效的标志
  Node *root_ = nullptr; // AST的根节点指针
  PStdLibs libs_; // Pascal标准库对象
};

// 叶子节点类，继承自Node，通常表示终结符或字面量
class LeafNode : public Node {
 public:
  // 默认构造函数
  LeafNode() {}
  // 带常量值的构造函数
  LeafNode(ConstValue val) : value_(val) {}

  // 获取标识符的字符串表示，如果是引用则添加 "(*...)"
  const std::string id_ref() {
    return is_ref_ ? "(*" + value_.get<std::string>() + ")"
                   : value_.get<std::string>();
  }
  // 设置叶子节点的值
  void set_value(ConstValue value) { value_ = value; }
  // 设置叶子节点是否为引用
  void set_ref(bool ref) { is_ref_ = ref; }
  // 模板函数，获取叶子节点的值
  template <typename T>
  T value() {
    return value_.get<T>();
  }
  // 获取值的基本类型
  BasicType *type() { return value_.type(); }

  // 分析引用关系 (具体实现在cpp文件中)
  bool AnalyzeReference(TableSet *ts, FunctionSymbol *fn);
  // 重写基类的Format方法，用于格式化输出叶子节点
  void Format(FILE *dst) override;

 private:
  ConstValue value_;     // 存储叶子节点的常量值
  bool is_ref_ = false;  // 标记此叶子节点是否表示一个引用
};

// 程序节点，表示整个Pascal程序
class ProgramNode : public Node {
  // Pascal文法: program → program_head program_body
};

// 程序头部节点
class ProgramHeadNode : public Node {
  // Pascal文法: programhead → program_id (idlists)
 public:
  // 格式化输出程序头部
  void Format(FILE *dst) override;
};

// 程序体节点
class ProgramBodyNode : public Node {
  // Pascal文法: program_body → const_declarations type_declarations var_declarations subprogram_declarations compound_statement
 public:
  // 格式化输出程序体
  void Format(FILE *dst) override;
};

// 标识符列表节点
class IdListNode : public Node {
 public:
  // 标识符列表的文法类型
  enum class GrammarType {
    SINGLE_ID,   // idlists → id (单个标识符)
    MULTIPLE_ID  // idlists → idlist,id (逗号分隔的多个标识符)
  };
  // 构造函数，传入文法类型
  IdListNode(GrammarType gt) : grammar_type_(gt) {}
  // 格式化输出标识符列表
  void Format(FILE *dst) override;
  // 获取列表中的所有叶子节点 (标识符)
  std::vector<LeafNode *> Lists();

 private:
  GrammarType grammar_type_; // 存储标识符列表的文法类型
};

// 常量声明块节点
class ConstDeclarationsNode : public Node {
  // Pascal文法: const_declarations → EPSILON | const_declaration
};

// 单个常量声明节点
class ConstDeclarationNode : public Node {
 public:
  // 常量声明的文法类型
  enum class GrammarType {
    VALUE,       // ConstDeclaration → id = const_var (单个常量定义)
    DECLARATION  // ConstDeclaration → ConstDeclaration; id = const_var (多个常量定义，递归)
  };
  // 构造函数，传入文法类型和基本类型
  ConstDeclarationNode(GrammarType gt, BasicType *bt)
      : grammar_type_(gt), type_(bt) {}
  // 打印类型信息到文件流
  void print_type(FILE *dst);
  // 格式化输出常量声明
  void Format(FILE *dst) override;

 private:
  GrammarType grammar_type_; // 存储常量声明的文法类型
  BasicType *type_; // 常量的类型
};

// 常量值节点
class ConstVariableNode : public Node {
  // Pascal文法: const_variable → +id | -id | id | +num | -num | num | 'letter'
};

// 变量声明块节点
class VariableDeclarationsNode : public Node {
  // Pascal文法: variable_declarations → EPSILON | var variable_declaration
};

// 单个变量声明节点
class VariableDeclarationNode : public Node {
 public:
  // 变量声明的文法类型
  enum class GrammarType {
    SINGLE_DECL,   // variable_declaration → idlist : xx (单个声明语句)
    MULTIPLE_DECL  // variable_declaration → variable_declaration ; idlist : xx (多个声明语句，递归)
  };
  // 列表类型，用于区分声明中的列表是标识符列表还是类型列表 (此编译器可能简化处理)
  enum class ListType { TYPE, ID };
  // 构造函数，传入文法类型和列表类型
  VariableDeclarationNode(GrammarType gt, ListType lt)
      : grammar_type_(gt), list_type_(lt) {}
  // 格式化输出变量声明
  void Format(FILE *dst) override;

 private:
  GrammarType grammar_type_; // 存储变量声明的文法类型
  ListType list_type_; // 存储列表类型
};

// 类型声明块节点
class TypeDeclarationsNode : public Node {
  // Pascal文法: type_declarations → EPSILON | type type_declaration
};

// 单个类型声明节点
class TypeDeclarationNode : public Node {
 public:
  // 类型声明的文法类型
  enum class GrammarType {
    SINGLE_DECL,   // id = type (单个类型定义)
    MULTIPLE_DECL  // TypeDeclaration→TypeDeclaration ; id = type (多个类型定义，递归)
  };
  // 构造函数，传入文法类型
  TypeDeclarationNode(GrammarType gt) : grammar_type_(gt) {}
  // 格式化输出类型声明
  void Format(FILE *dst) override;

 private:
  GrammarType grammar_type_; // 存储类型声明的文法类型
};

// 类型定义节点 (如 integer, array, record)
class TypeNode : public Node {
 public:
  // 类型的文法种类
  enum class GrammarType { BASIC_TYPE, ARRAY, RECORD_TYPE };
  // 构造函数，传入文法种类
  TypeNode(GrammarType gt) : grammar_type_(gt) {}

  // 获取类型的文法种类
  GrammarType grammar_type() { return grammar_type_; }
  // 设置基类型节点 (例如数组的元素类型)
  void set_base_type_node(TypeNode *node) { base_type_node_ = node; }
  // 获取基类型节点
  TypeNode *base_type() { return base_type_node_; }

  // 格式化输出数组的维度周期
  void PeriodsFormat(FILE *dst);
  // 格式化输出类型定义
  void Format(FILE *dst) override;

 private:
  GrammarType grammar_type_; // 存储类型的文法种类
  TypeNode *base_type_node_; // 指向基类型的指针 (用于数组等复合类型)
};

// 基本类型节点 (integer, real, boolean, char)
class BasicTypeNode : public Node {
  // Pascal文法: BasicType →integer|real|boolean|char
 public:
  // 默认构造函数
  BasicTypeNode() {}
  // 带类型指针的构造函数
  BasicTypeNode(BasicType *type) : type_(type) {}

  // 获取C语言风格的类型名称，ref为true时表示指针
  std::string TypeName(bool ref = false) {
    std::string type_name = type_->c_name() + (ref ? "*" : "");
    return type_name;
  }
  // 设置基本类型
  void set_type(BasicType *type) { type_ = type; }
  // 获取基本类型
  BasicType *type() { return type_; }
  // 格式化输出基本类型
  void Format(FILE *dst) override;

 private:
  BasicType *type_; // 指向实际基本类型信息的指针
};

// 记录体定义节点
class RecordBodyNode : public Node {
  // Pascal文法: record_body → EPSILON | var_declaration (记录的字段声明)
};

// 数组维度周期列表节点
class PeriodsNode : public Node {
  // Pascal文法: periods → period | periods, period
};

// 数组单个维度周期节点
class PeriodNode : public Node {
  // Pascal文法: Period → const_var ... const var (如 1..10)
 public:
  // 格式化输出维度周期
  void Format(FILE *dst) override;
  // 获取维度长度
  int len() { return len_; }
  // 设置维度长度
  void set_len(int len) { len_ = len; }

 private:
  int len_; // 存储维度长度
};

// 子程序声明块节点 (过程和函数)
class SubprogramDeclarationsNode : public Node {
  // Pascal文法: subprogram_declarations → EPSILON | subprogram_declarations subprogram_declaration
};

// 单个子程序声明节点
class SubprogramDeclarationNode : public Node {
  // Pascal文法: subprogram_declaration -> subprogram_head subprogram_body
 public:
  // 格式化输出子程序声明
  void Format(FILE *dst) override;
};

// 子程序体节点
class SubprogramBodyNode : public Node {
  // Pascal文法: subprogram_body → const_declarations type_declarations var_declarations compound_statement
 public:
};

// 子程序头部节点
class SubprogramHeadNode : public Node {
 public:
  // 子程序头部的文法类型 (过程或函数)
  enum class GrammarType {
    PROCEDURE,  // subprogram_head → procedure id formal_parameter
    FUNCTION    // subprogram_head → function id formal_parameter : basic_type
  };
  // 构造函数，传入文法类型
  SubprogramHeadNode(GrammarType gt) : grammar_type_(gt) {}
  // 格式化输出子程序头部
  void Format(FILE *dst) override;
  // 获取子程序的文法类型
  GrammarType grammar_type() { return grammar_type_; }
  // 设置子程序标识符
  void set_id(std::string id) { id_ = id; }
  // 获取子程序标识符
  std::string id() { return id_; }

 private:
  std::string id_; // 子程序名称
  GrammarType grammar_type_; // 过程或函数
};

// 形式参数节点
class FormalParamNode : public Node {
 public:
  // Pascal文法: formal_parameter → EPSILON | ( parameter_lists )
  // 格式化输出形式参数
  void Format(FILE *dst) override;
};

// 参数列表节点
class ParamListsNode : public Node {
 public:
  // 参数列表的文法类型
  enum class GrammarType {
    SINGLE_PARAM_LIST,    // param_lists→ param_list (单个参数段)
    MULTIPLE_PARAM_LIST,  // param_lists→ param_lists ; param_list (多个参数段，以分号分隔)
  };
  // 构造函数，传入文法类型
  ParamListsNode(GrammarType gt) : grammar_type_(gt) {}
  // 格式化输出参数列表
  void Format(FILE *dst) override;

 private:
  GrammarType grammar_type_; // 存储参数列表的文法类型
};

// 单个参数段节点
class ParamListNode : public Node {
  // Pascal文法: parameter_list → var_parameter | value_parameter (变量参数或值参数)
};

// 变量参数节点 (传引用)
class VarParamNode : public Node {
  // Pascal文法: var_param → var value_param
 public:
  // 格式化输出变量参数
  void Format(FILE *dst) override;
};

// 值参数节点 (传值)
class ValueParamNode : public Node {
  // Pascal文法: ValueParam → idlist : basic_type
 public:
  // 格式化输出值参数
  void Format(FILE *dst) override;
  // 带引用标志的格式化输出 (用于区分实际是传值还是传地址)
  void Format(bool ref, FILE *dst);
};

// 复合语句节点 (begin ... end)
class CompoundStatementNode : public Node {
  // Pascal文法: CompoundStatement → begin StatementList end
};

// 语句列表节点
class StatementListNode : public Node {
  // Pascal文法: statement_list → statement | statement_list ; statement
};

// 语句节点
class StatementNode : public Node {
 public:
  // 语句的文法类型
  enum class GrammarType {
    EPSILON,             // 空语句
    VAR_ASSIGN_OP_EXP,   // 变量赋值语句
    FUNC_ASSIGN_OP_EXP,  // 函数结果赋值语句
    PROCEDURE_CALL,      // 过程调用语句
    COMPOUND_STATEMENT,  // 复合语句
    IF_STATEMENT,        // IF语句
    FOR_STATEMENT,       // FOR语句
    READ_STATEMENT,      // READ语句
    READLN_STATEMENT,    // READLN语句
    WRITE_STATEMENT,     // WRITE语句
    WRITELN_STATEMENT,   // WRITELN语句
    CASE_STATEMET,       // CASE语句
    WHILE_STATEMENT,     // WHILE语句
    REPEAT_STATEMENT     // REPEAT-UNTIL语句
  };
  // 构造函数，传入文法类型
  StatementNode(GrammarType gt) : grammar_type_(gt) {}
  // 格式化输出语句
  void Format(FILE *dst) override;

 private:
  GrammarType grammar_type_; // 存储语句的文法类型
};

// 变量列表节点 (用于read, readln等)
class VariableListNode : public Node {
 public:
  // 变量列表的文法类型
  enum class GrammarType {
    VARIABLE,                // variable_list → variable (单个变量)
    VARIABLE_LIST_VARIABLE,  // variable_list → variable_list , variable (逗号分隔的多个变量)
  };
  // 构造函数，传入文法类型
  VariableListNode(GrammarType gt) : grammar_type_(gt) {}
  // 将变量列表格式化为字符串 (可能用于printf的格式串)
  std::string FormatString();
  // 格式化输出变量列表 (可能用于scanf的参数)
  void Format(FILE *dst) override;
  // 带引用标志的格式化输出
  void Format(bool ref, FILE *dst);
  // 设置变量列表的类型信息
  bool set_types(std::vector<TypeTemplate *> *type_list);

 private:
  std::vector<BasicType *> basic_types; // 存储列表中各变量的基本类型
  GrammarType grammar_type_; // 存储变量列表的文法类型
};

// 变量引用节点 (如 a, a[i], r.f)
class VariableNode : public Node {
  // Pascal文法: variable → id id_varparts
 public:
  // 带引用标志的格式化输出变量
  void Format(bool ref, FILE *dst);
};

// 标识符变量部分节点 (数组下标或记录成员访问)
class IDVarPartNode : public Node {
 public:
  // 变量部分的文法类型
  enum class GrammarType {
    _ID,       // id_varpart → .id (记录成员访问)
    EXP_LIST,  // id_varpart → [ expression_list ] (数组元素访问)
  };
  // 构造函数，传入文法类型
  IDVarPartNode(GrammarType gt) : grammar_type_(gt) {}
  // 获取变量部分的文法类型
  GrammarType grammar_type() { return grammar_type_; }
  // 格式化输出变量部分
  void Format(FILE *dst) override;
  // 设置数组的下界 (用于多维数组地址计算)
  void set_array_lb(int lb) { array_lb_ = lb; }

 private:
  int array_lb_ = 0; // 数组下界，默认为0
  GrammarType grammar_type_; // 存储变量部分的文法类型
};

// 标识符变量部分链节点 (处理连续的 .id 或 [exp])
class IDVarPartsNode : public Node {
 public:
  // Pascal文法: id_varparts → EPSILON | id_varparts id_varpart
  // 设置多维数组各维度的下界
  void set_lb(std::vector<ArrayType::ArrayBound> &bound) {
    if (child_list_.size() == 0) return; // 如果没有子部分，则返回
    // 递归处理更深层次的IDVarPartsNode
    child_list_[0]->DynamicCast<IDVarPartsNode>()->set_lb(bound);
    // 如果当前部分是数组下标表达式列表
    if (child_list_[1]->DynamicCast<IDVarPartNode>()->grammar_type() ==
        IDVarPartNode::GrammarType::EXP_LIST) {
      // 设置该维度的下界
      child_list_[1]->DynamicCast<IDVarPartNode>()->set_array_lb(bound[0].lb_);
      bound.erase(bound.begin()); // 移除已处理的维度边界信息
    }
  }
};

// Case语句分支列表节点
class BranchListNode : public Node {
  // Pascal文法: branchlist → branch | branchlist branch
};

// Case语句体节点
class CaseBodyNode : public Node {
  // Pascal文法: case_body → branch_list | EPSILON
};

// Case语句单个分支节点
class BranchNode : public Node {
  // Pascal文法: branch → const_list : statement
 public:
  // 格式化输出Case分支
  void Format(FILE *dst) override;
};

// 常量列表节点 (用于Case分支)
class ConstListNode : public Node {
 public:
  // Pascal文法: constlist → const_variable | constlist , const_variable
  // 格式化输出常量列表
  void Format(FILE *dst) override;
};

// For循环方向节点 (to 或 downto)
class UpdownNode : public Node {
 public:
  // 构造函数，默认为递增 (to)
  UpdownNode(bool is_increase = true) : is_increase_(is_increase) {}
  // 判断是否为递增
  bool IsIncrease() { return is_increase_; }
  // 设置循环方向
  void set_increase(bool inc) { is_increase_ = inc; }

 private:
  bool is_increase_; // true表示to (递增), false表示downto (递减)
};

// 过程调用节点
class ProcedureCallNode : public Node {
 public:
  // 过程调用的文法类型
  enum class GrammarType {
    ID,           // procedure_call → id (无参数调用)
    ID_EXP_LIST,  // procedure_call → id ( expression_list ) (带参数列表调用)
  };
  // 构造函数，传入文法类型
  ProcedureCallNode(GrammarType gt) : grammar_type_(gt) {}
  // 格式化输出过程调用
  void Format(FILE *dst) override;

 private:
  GrammarType grammar_type_; // 存储过程调用的文法类型
};

// Else子句节点 (用于IF语句)
class ElseNode : public Node {
 public:
  // Else子句的文法类型
  enum class GrammarType {
    EPSILON,         // else_part → EPSILON (无else)
    ELSE_STATEMENT,  // else_part → else statement (有else语句)
  };
  // 构造函数，传入文法类型
  ElseNode(GrammarType gt) : grammar_type_(gt) {}
  // 格式化输出Else子句
  void Format(FILE *dst) override;
  // 获取Else子句中的语句节点
  StatementNode *GetStatement() {
    return child_list_[0]->DynamicCast<StatementNode>();
  }

 private:
  GrammarType grammar_type_; // 存储Else子句的文法类型
};

// 表达式列表节点
class ExpressionListNode : public Node {
 public:
  // 表达式列表的文法类型
  enum class GrammarType {
    EXP,           // expression_list → expression (单个表达式)
    EXP_LIST_EXP,  // expression_list → expression_list , expression (逗号分隔的多个表达式)
  };
  // 构造函数，传入文法类型
  ExpressionListNode(GrammarType gt) : grammar_type_(gt) {}
  // 将表达式列表格式化为字符串
  std::string FormatString();
  // 格式化输出表达式列表
  void Format(FILE *dst) override;
  // 设置表达式列表中各表达式的类型信息
  bool set_types(std::vector<TypeTemplate *> *type_list);
  // 设置引用状态栈 (用于处理函数参数传递等)
  void set_ref(std::stack<bool> *ref);

 private:
  std::vector<BasicType *> basic_types; // 存储列表中各表达式的基本类型
  GrammarType grammar_type_; // 存储表达式列表的文法类型
};

// 表达式节点
class ExpressionNode : public Node {
  // Pascal文法: expression → str_expression | simple_expression | simple_expression relop simple_expression
 public:
  // 表达式的目标代码生成类型 (用于优化或特殊处理)
  enum class TargetType {
    EXPRESSION, // 普通表达式
    VAR_ARRAY, // 数组变量 (可能用于地址计算)
    CONST_STRING, // 字符串常量
  };
  // 默认构造函数，目标类型为普通表达式
  ExpressionNode() : target_type_(TargetType::EXPRESSION) {}
  // 带目标类型的构造函数
  ExpressionNode(TargetType tg) : target_type_(tg) {}
  // 标记表达式结果是否为引用
  void set_is_ref() { is_ref_ = 1; }
  // 设置表达式的目标类型
  void set_expression_type(TargetType tg) { target_type_ = tg; }
  // 获取表达式的目标类型
  TargetType target_type() { return target_type_; }
  // 格式化输出表达式
  void Format(FILE *dst) override;

 private:
  TargetType target_type_; // 表达式的目标类型
  bool is_ref_ = 0; // 标记表达式结果是否为引用 (例如函数返回引用)
};

// 字符串表达式节点 (Pascal中字符串处理可能较为特殊)
class StrExpressionNode : public Node {
  // Pascal文法: str_expression -> str_ ## [leaf] (可能是自定义的字符串连接操作)
  //             str_expression -> str_expression + str_
 public:
  // 格式化输出字符串表达式
  void Format(FILE *dst) override;
};

// 简单表达式节点 (通常指算术表达式中的加减部分)
class SimpleExpressionNode : public Node {
  // Pascal文法: simple_expression → term | +term | -term | simple_expression addop term
};

// 项节点 (通常指算术表达式中的乘除部分)
class TermNode : public Node {
 public:
  // Pascal文法: term → factor | term mulop factor
  // 标记当前项的操作是否为除法
  void set_op_div(bool op) { op_div = op; }
  // 格式化输出项
  void Format(FILE *dst) override;

 private:
  bool op_div = false; // true表示此项涉及除法运算
};

// 因子节点 (表达式的最小单元)
class FactorNode : public Node {
 public:
  // 因子的文法类型
  enum class GrammarType {
    UCONST_VAR,   // factor → unsigned_const_variable (无符号常量或变量)
    VARIABLE,     // factor → variable (变量引用)
    EXP,          // factor → ( expression ) (括号括起的表达式)
    ID_EXP_LIST,  // factor → id ( expression_list ) (函数调用)
    NOT,          // factor → not factor (逻辑非)
  };
  // 构造函数，传入文法类型
  FactorNode(GrammarType gt) : grammar_type_(gt) {}
  // 格式化输出因子
  void Format(FILE *dst) override;

 private:
  GrammarType grammar_type_; // 存储因子的文法类型
};

// 无符号常量或变量节点 (如数字、字符、枚举标识符等)
class UnsignConstVarNode : public Node {
  // Pascal文法: unsigned_const_variable → num | 'letter' | id (如果id是常量)
};

}  // namespace ast
}  // namespace pascals
#endif
