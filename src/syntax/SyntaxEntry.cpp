﻿#include "syntax/SyntaxEntry.h"


using namespace syntax;
using namespace token;


static Token lhs[] = {
        // program
        real_start,   // 0
        prog,         // 1
        prog_head,    // 2
        prog_head,    // 3
        prog_body,    // 4

        // sub program 5-10
        sub_prog_def,   // 5
        sub_prog_def,   // 6
        sub_prog,       // 7
        sub_prog_body,   // 8
        sub_prog_head,   // 9
        sub_prog_head,   // 10

        // parameter
        formal_para,    // 11
        formal_para,    // 12
        formal_para,    // 13
        para_list,      // 14
        para_list,      // 15
        para,          // 16
        para,         // 17
        var_para,      // 18
        value_para,    // 19

        // const
        const_defs,    // 20
        const_defs,    // 21
        const_def,    // 22
        const_def,     // 23

        cont,        // 24
        cont,       // 25
        cont,       // 26
        cont,     // 27

        // var
        var_defs,       // 28
        var_defs,       // 29
        var_def,    // 30
        var_def,    // 31
        var_with_type,   // 32
        var_with_type,    // 33

        // type
        type,    // 34
        type,    // 35
        type_base,   // 36
        type_base,    // 37
        type_base,  //  38
        type_base,    // 39
        period_with_type,    // 40
        period_with_type,    // 41

        // id 42-47
        id_with_type,    // 42
        id_with_type,    // 43
        var,        // 44
        var,       // 45
        var_list,    // 46
        var_list,     // 47

        // op  48-60
        op_cmp,          // 48
        op_cmp,      // 49
        op_cmp,     // 50
        op_cmp,     // 51
        op_cmp,     // 52
        op_cmp,     // 53

        op_add_sub,   // 54
        op_add_sub,    // 55
        op_add_sub,   // 56

        op_div_mul,    // 57
        op_div_mul,    // 58
        op_div_mul,    // 59
        op_div_mul,    // 60

        // statement
        main,       // 61
        begin,    // 62
        stmt_list,    // 63
        stmt_list,    // 64

        stmt,    // 65
        stmt,    // 66

        stmt_base,
        stmt_base,
        stmt_base,
        stmt_base,
        stmt_base,
        stmt_base,
        stmt_base,
        stmt_base,
        stmt_base,


        proc_call,
        proc_call,
        proc_call,

        else_part,
        else_part,

        // expression
        exp_list,
        exp_list,

        TokenState::exp,
        TokenState::exp,


        // sub_exp
        sub_exp,
        sub_exp,


        // term
        term,
        term,


        // factor
        factor,
        factor,
        factor,
        factor,
        factor,
        factor,
        factor,
        factor,
        factor,

};

static std::vector<Token> rhs[] = {

        // program
        {prog,          real_end},
        {prog_head,     p_semicolon, prog_body,    p_dot},
        {key_prog,      id},
        {key_prog,      id,          p_l_paren,    id,       p_r_paren},
        {const_defs,    var_defs,    sub_prog_def, main},

        // sub program 5-10
        {},
        {sub_prog,      p_semicolon, sub_prog_def},
        {sub_prog_head, p_semicolon, sub_prog_body},
        {const_defs,    var_defs,    main},
        {key_proc,      idf,          formal_para},
        {key_func,      idf,          formal_para,  p_colon,   type_base},

        // parameter
        {},
        {p_l_paren,     para_list,   p_r_paren},
        {p_l_paren,     p_r_paren},
        {para},
        {para,          p_semicolon, para_list},
        {var_para},
        {value_para},
        {key_var,       value_para},
        {id_with_type},

        // const  20-27
        {},
        {key_const,     const_def,   p_semicolon},
        {id,            op_equal,    cont},
        {const_def,     p_semicolon, id,           op_equal,  cont},

        {letter},
        {op_neg,        num},
        {op_pos,        num},
        {num},

        // var 28-33
        {},
        {key_var,       var_def,     p_semicolon},
        { var_with_type},
        { var_def, p_semicolon, var_with_type},
        { id, p_comma, var_with_type},
        { id, p_colon, type},

        // type  34-41
        {type_base},
        {type_array,    op_l_squ,    period_with_type},
        {type_int},
        {type_real},
        {type_char},
        {type_bool},
        {digit,         p_dotdot,    digit,        p_comma, period_with_type},
        {digit,         p_dotdot,    digit,       op_r_squ,  key_of, type_base},

        // id 42-47
        {id, p_colon, type_base},
        {id,            p_comma,     id_with_type},
        {id},
        {id,            op_l_squ,    exp_list,     op_r_squ},
        {var},
        {var,           p_comma,     var_list},

        // op
        {op_great},
        {op_less},
        {op_equal},
        {op_great_equ},
        {op_less_equ},
        {op_not_equ},

        {op_add},
        {op_sub},
        {op_or},

        {op_div},
        {op_mul},
        {op_mod},
        {op_and},

        // statement
        {begin,         stmt_list,   key_end},
        {key_begin},
        {stmt},
        {stmt_list,     p_semicolon, stmt},

        {},
        {stmt_base},

        {main},
        {proc_call},
        {var,           op_assign,   TokenState::exp},
        {idf,           op_assign,   TokenState::exp},
        {key_read,      p_l_paren,   var_list,     p_r_paren},
        {key_write,     p_l_paren,   exp_list,     p_r_paren},
        {key_for,       id,          op_assign,    TokenState::exp,       key_to, TokenState::exp, key_do, stmt_base},
        {key_while,     TokenState::exp,         key_do, stmt_base},
        {key_if,        TokenState::exp,         key_then,     stmt, else_part},

        {idf},
        {idf,            p_l_paren,   exp_list,     p_r_paren},
        {idf,            p_l_paren,   p_r_paren},

        {},
        {key_else,      stmt_base},

        // exp
        {TokenState::exp},
        {exp_list,      p_comma,     TokenState::exp},

        {sub_exp},
        {TokenState::exp,           op_cmp,      sub_exp},


        // sub_exp
        {term},
        {sub_exp,       op_add_sub,  term},


        // term
        {factor},
        {term,          op_div_mul,  factor},


        // factor
        {num},
        {var},
        {p_l_paren,     TokenState::exp,         p_r_paren},
        {idf,            p_l_paren,   exp_list,     p_r_paren},
        {idf,            p_l_paren,   p_r_paren},
        {idf},

        {op_not,        factor},
        {op_neg,        factor},
        {op_pos,        factor},

};

static_assert(sizeof(lhs) / sizeof(Token) == sizeof(rhs) / sizeof(std::vector<Token>));

const int entryCount = sizeof(lhs) / sizeof(Token);

inline SyntaxArray &syntax::getSyntaxes() {
    static SyntaxArray syntaxes;
    return syntaxes;
}

syntax::SyntaxEntry::SyntaxEntry(size_t id) :
        id(id),
        l(lhs[id]),
        r(rhs[id]) {}

void syntax::initSyntaxes() {
    for (int i = 0; i < entryCount; ++i)
        getSyntaxes().emplace_back(i);
}
