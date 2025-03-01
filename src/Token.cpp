#include "Token.h"

using namespace std;
using namespace token;

std::string token::tokenNames[] = {
    "real_start",
    
    "prog",
    "prog_head",
    "prog_body",
    
    "var_body",
    "var_def",
    "const_body",
    "const_def",
    "main",
    "begin",
    "stmt",
    "stmt_list",
    
    "exp_list",
    "exp",
    "sub_exp",
    "term",
    "factor",
    
    "var",
    "var_list",
    "cont",
    
    "type_base",
    "type",
    
    "ids",
    "op_cmp",
    "op_add_sub",
    "op_div_mul",

    // terminal
    
    "int",
    "real",
    "char",
    "array",
    "boolean",
    
    "id",
    "number",
    "str",
    
    "{",
    "}",
    "(",
    ")",
    ":",
    "'",
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
    
    "for",
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