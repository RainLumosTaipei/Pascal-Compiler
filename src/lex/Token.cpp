#include "lex/Token.h"

using namespace std;
using namespace token;

std::string token::tokenNames[] = {
    "real_start",
    
    "prog",
    "prog_head",
    "prog_body",

    "sub_prog_def",            // sub program declare
    "sub_prog",                // sub program
    "sub_prog_head",           // sub program head
    "sub_prog_body",

    "var_para",
    "value_para",
    "para",
    "para_list",
    "formal_para",

    "var_def",
    "var_defs",
    "var_with_type",
    "const_def",
    "const_defs",
    "main",
    "begin",
    "stmt",
    "stmt_base",
    "stmt_list",
    
    "exp_list",
    "exp",
    "sub_exp",
    "term",
    "factor",
    "else-part",
    "proc-call",
    
    "var",
    "var_list",
    "cont",
    
    "type_base",
    "type",
    "period",
    "period_with_type",
    "id_with_type",
    "op_cmp",
    "op_add_sub",
    "op_div_mul",

    // terminal
    // type
    "int",
    "real",
    "char",
    "array",
    "boolean",
    // value
    "id",
    "idf",
    "number",
    "str",
    "digit",

    "{",
    "}",
    "(",
    ")",
    ":",
    ",",
    ";",
    "..",
    ".",
    
    "=",
    "+",
    "-",
    "*",
    "/",
    "<>",
    "[",
    "]",
    ":=",
    "mod",
    "<",
    "<=",
    ">",
    ">=",
    "and",
    "or",
    "not",
    "neg",
    "pos",

    "for",
    "while",
    "if",
    "else",
    "then",
    "to",
    "do",
    "of",
    "const",
    "var",
    "begin",
    "end",
    "program",
    "procedure",
    "function",
    "read",
    "write",
    
    "null",
    "$"
};

static_assert(tokenCount == sizeof(tokenNames) / sizeof(string));