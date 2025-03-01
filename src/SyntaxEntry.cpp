#include "SyntaxEntry.h"

using namespace std;
using namespace syntax;
using namespace token;


static Token lhs[] = {

    TokenState::real_start,
    
    TokenState::prog,
    TokenState::prog_head,
    TokenState::prog_body,
    
    TokenState::const_body,
    TokenState::const_body,
    TokenState::const_def,
    TokenState::const_def,
    
    TokenState::var_body,
    TokenState::var_body,
    TokenState::var_def,
    TokenState::var_def,
    
    TokenState::type,
    TokenState::type_base,
    TokenState::type_base,
    //TokenState::type_base,
    //TokenState::type_base,
    
    TokenState::ids,
    TokenState::ids,
    
    TokenState::main,
    TokenState::begin,
    TokenState::stmt_list,
    TokenState::stmt_list,
    
    TokenState::stmt,
    TokenState::variable,
    TokenState::variable,
    //TokenState::cont,
    //TokenState::cont,
    //TokenState::cont,
    TokenState::cont,
    //TokenState::cont,
    //TokenState::cont,
    TokenState::cont,

    TokenState::exp,
    TokenState::exp,
    TokenState::exp,
    TokenState::exp,
    //TokenState::exp,
    //TokenState::exp,
    //TokenState::exp,

    TokenState::subexp,
    TokenState::subexp,
    TokenState::subexp,
    //TokenState::subexp,
    
    TokenState::term,
    TokenState::factor,
    TokenState::factor,
    TokenState::factor,
    //TokenState::factor,
    
    //TokenState::factor,
    //TokenState::factor,
    TokenState::factor,
    TokenState::factor,
    TokenState::factor,
    
};

static vector<Token> rhs[] = {

    { TokenState::prog,       TokenState::real_end },
    
    { TokenState::prog_head,  TokenState::prog_body },
    { TokenState::key_prog,   TokenState::id,         TokenState::p_semicolon },
    { TokenState::const_body, TokenState::var_body,   TokenState::main },

    { },
    { TokenState::key_const,  TokenState::const_def},
    { TokenState::id,         TokenState::op_equal,   TokenState::cont,     TokenState::p_semicolon },
    { TokenState::const_def,  TokenState::id,         TokenState::op_equal, TokenState::cont, TokenState::p_semicolon},

    { },
    { TokenState::key_var,    TokenState::var_def},
    { TokenState::ids,        TokenState::p_colon, TokenState::type,     TokenState::p_semicolon },
    { TokenState::var_def,    TokenState::ids,     TokenState::p_colon,  TokenState::type, TokenState::p_semicolon },

    { TokenState::type_base },
    { TokenState::type_int},
    { TokenState::type_real},
    //{ TokenState::type_char},
    //{ TokenState::type_bool},

    { TokenState::id },
    { TokenState::ids,        TokenState::p_comma, TokenState::id },
    
    { TokenState::begin, TokenState::stmt_list,    TokenState::key_end, TokenState::p_dot },
    { TokenState::key_begin },
    { TokenState::stmt },
    { TokenState::stmt_list, TokenState::stmt },
    
    { TokenState::variable, TokenState::op_assign, TokenState::exp, TokenState::p_semicolon },
    { TokenState::id},
    { TokenState::id, TokenState::op_l_square,     TokenState::exp, TokenState::op_r_square},
    //{TokenState::p_quote, TokenState::character, TokenState::p_quote},
    //{ TokenState::op_add, TokenState::integer},
    //{ TokenState::op_minus, TokenState::integer},
    {  TokenState::integer},
    //{ TokenState::op_add, TokenState::real},
    //{ TokenState::op_minus, TokenState::real},
    {  TokenState::real},

    { TokenState::subexp},
    { TokenState::subexp, TokenState::op_great, TokenState::subexp},
    { TokenState::subexp, TokenState::op_less, TokenState::subexp},
    { TokenState::subexp, TokenState::op_equal, TokenState::subexp},
    //{ TokenState::subexp, TokenState::op_great_equal, TokenState::subexp},
    //{ TokenState::subexp, TokenState::op_less_equal, TokenState::subexp},
    //{ TokenState::subexp, TokenState::op_not_equal, TokenState::subexp},

    { TokenState::term},
    { TokenState::term, TokenState::op_add, TokenState::term},
    { TokenState::term, TokenState::op_minus, TokenState::term},
    //{ TokenState::term, TokenState::op_or, TokenState::term},

    { TokenState::factor},
    { TokenState::factor,    TokenState::op_mul, TokenState::factor},
    { TokenState::factor,    TokenState::op_div, TokenState::factor},
    { TokenState::factor,    TokenState::op_mod, TokenState::factor},
    //{ TokenState::factor, TokenState::op_and, TokenState::factor},

    //{ TokenState::op_not, TokenState::factor},
    //{ TokenState::op_minus, TokenState::factor},
    { TokenState::p_l_paren, TokenState::exp,    TokenState::p_r_paren},
    { TokenState::variable},
    { TokenState::cont}
    
};

static_assert(sizeof(lhs) / sizeof(Token) == sizeof(rhs) / sizeof(vector<Token>));

const int entryCount = sizeof(lhs) / sizeof(Token);

inline SyntaxArray& syntax::getSyntaxes(){
    static SyntaxArray syntaxes;
    return syntaxes;
}

syntax::SyntaxEntry::SyntaxEntry(size_t id) :
        id(id),
        l(lhs[id]),
        r(rhs[id]) {}

void syntax::initSyntaxes()
{
    for (int i = 0; i < entryCount; ++i)
        getSyntaxes().emplace_back(i);
}
