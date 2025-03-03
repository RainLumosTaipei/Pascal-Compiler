#include "SyntaxEntry.h"


using namespace syntax;
using namespace token;


static Token lhs[] = {
        // program
        real_start,
        prog,
        prog_head,
        prog_head,
        prog_body,

        // sub program
        sub_prog_def,
        sub_prog_def,
        sub_prog,
        sub_prog_body,
        sub_prog_head,
        sub_prog_head,

        // parameter
        formal_para,
        formal_para,
        formal_para,
        para_list,
        para_list,
        para,
        para,
        var_para,
        value_para,

        // const
        const_defs,
        const_defs,
        const_def,
        const_def,

        cont,
        cont,
        cont,
        cont,

        // var
        var_defs,
        var_defs,
        var_def,
        var_def,

        // type
        type,
        type,
        type_base,
        type_base,
        type_base,
        type_base,
        period,
        period,

        // id
        ids,
        ids,
        var,
        var,
        var_list,
        var_list,

        // op
        op_cmp,
        op_cmp,
        op_cmp,
        op_cmp,
        op_cmp,
        op_cmp,

        op_add_sub,
        op_add_sub,
        op_add_sub,

        op_div_mul,
        op_div_mul,
        op_div_mul,
        op_div_mul,

        // statement
        main,
        begin,
        stmt_list,
        stmt_list,

        stmt,
        stmt,

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

        exp,
        exp,


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
        {key_prog,      id,          p_l_paren,    ids,       p_r_paren},
        {const_defs,    var_defs,    sub_prog_def, main},

        // sub program
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
        {ids,           p_colon,     type_base},

        // const
        {},
        {key_const,     const_def,   p_semicolon},
        {id,            op_equal,    cont},
        {const_def,     p_semicolon, id,           op_equal,  cont},

        {letter},
        {op_neg,        num},
        {op_pos,        num},
        {num},

        // var
        {},
        {key_var,       var_def,     p_semicolon},
        {ids,           p_colon,     type},
        {var_def,       p_semicolon, ids,          p_colon,   type},

        // type
        {type_base},
        {type_array,    op_l_squ,    period,       op_r_squ,  key_of, type_base},
        {type_int},
        {type_real},
        {type_char},
        {type_bool},
        {digit,         p_dotdot,    digit},
        {digit,         p_dotdot,    digit,        p_comma,   period},

        // id
        {id},
        {id,            p_comma,     ids},
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
        {var,           op_assign,   exp},
        {idf,           op_assign,   exp},
        {key_read,      p_l_paren,   var_list,     p_r_paren},
        {key_write,     p_l_paren,   exp_list,     p_r_paren},
        {key_for,       id,          op_assign,    exp,       key_to, exp, key_do, stmt_base},
        {key_while,     exp,         key_do, stmt_base},
        {key_if,        exp,         key_then,     stmt, else_part},

        {idf},
        {idf,            p_l_paren,   exp_list,     p_r_paren},
        {idf,            p_l_paren,   p_r_paren},

        {},
        {key_else,      stmt_base},

        // exp
        {exp},
        {exp_list,      p_comma,     exp},

        {sub_exp},
        {exp,           op_cmp,      sub_exp},


        // sub_exp
        {term},
        {sub_exp,       op_add_sub,  term},


        // term
        {factor},
        {term,          op_div_mul,  factor},


        // factor
        {num},
        {var},
        {p_l_paren,     exp,         p_r_paren},
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
