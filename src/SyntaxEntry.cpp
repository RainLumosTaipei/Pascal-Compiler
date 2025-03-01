#include "SyntaxEntry.h"

using namespace std;
using namespace syntax;
using namespace token;


static Token lhs[] = {
        // program
        real_start,
        prog,
        prog_head,
        prog_body,

        // const
        const_body,
        const_body,
        const_def,
        const_def,

        cont,
        cont,
        cont,
        cont,

        // var
        var_body,
        var_body,
        var_def,
        var_def,

        // type
        type,
        type_base,
        type_base,
        type_base,
        type_base,

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
        TokenState::begin,
        stmt_list,
        stmt_list,

        stmt,
        stmt,
        stmt,
        stmt,
        stmt,

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

};

static vector<Token> rhs[] = {

        // program
        {prog,              real_end},
        {prog_head,         prog_body},
        {key_prog,          id,          p_semicolon},
        {const_body,        var_body,    main},

        // const
        {},
        {key_const,         const_def},
        {id,                op_equal,    cont,     p_semicolon},
        {const_def,         id,          op_equal, cont, p_semicolon},

        {p_quote,           letter,      p_quote},
        {op_add,            num},
        {op_sub,            num},
        {num},

        // var
        {},
        {key_var,           var_def},
        {ids,               p_colon,     type,     p_semicolon},
        {var_def,           ids,         p_colon,  type, p_semicolon},

        // type
        {type_base},
        {type_int},
        {type_real},
        {type_char},
        {type_bool},

        // id
        {id},
        {ids,               p_comma,     id},
        {id},
        {id,                op_l_square, exp_list, op_r_square},
        {var},
        {var_list,          p_comma,     var},

        // op
        {op_great},
        {op_less},
        {op_equal},
        {op_great_equal},
        {op_less_equal},
        {op_not_equal},

        {op_add},
        {op_sub},
        {op_or},

        {op_div},
        {op_mul},
        {op_mod},
        {op_and},

        // statement
        {TokenState::begin, stmt_list,   key_end,  p_dot},
        {key_begin},
        {stmt},
        {stmt_list,         p_semicolon, stmt},

        {},
        {main},
        {var,               op_assign,   exp},
        {key_read,          p_l_paren,   var_list, p_r_paren},
        {key_write,         p_l_paren,   exp_list, p_r_paren},


        // exp
        {exp},
        {exp_list,          p_comma,     exp},

        {sub_exp},
        {sub_exp,           op_cmp,      sub_exp},

        // sub_exp
        {term},
        {sub_exp,           op_add_sub,  term},

        // term
        {factor},
        {term,              op_div_mul,  factor},

        // factor
        {num},
        {var},
        {p_l_paren,         exp,         p_r_paren},
        {id,                p_l_paren,   exp_list, p_r_paren},

        {op_not,            factor},
        {op_sub,            factor},

};

static_assert(sizeof(lhs) / sizeof(Token) == sizeof(rhs) / sizeof(vector<Token>));

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
