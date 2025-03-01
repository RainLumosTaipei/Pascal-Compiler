#include "Token.h"

using namespace std;
using namespace token;

std::string token::tokenNames[] = {
    "real_start",
    
    "prog",
    "prog_head",
    "prog_body",
    
    "var_body",
    "var_d",
    "const_body",
    "const_d",
    "main",
    "begin",
    "stmt",
    "stmt_list",
    
    "exp",
    "subexp",
    "term",
    "factor",
    
    "variable",
    "cont",
    
    "type_base",
    "type",
    
    "ids",

    // terminal
    
    "int",
    "real",
    "char",
    "array",
    "boolean",
    
    "id",
    "100",
    "0.1f",
    "abc",
    
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