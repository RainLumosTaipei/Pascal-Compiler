#include "syntax/SyntaxEntry.h"


using namespace syntax;
using namespace token;


static Token lhs[] = {
    // program
    real_start, // 0
    prog, // 1
    prog_head, // 2
    prog_head, // 3
    prog_body, // 4

    // sub program 5-10
    sub_prog_def, // 5
    sub_prog_def, // 6
    sub_prog, // 7
    sub_prog_body, // 8
    sub_prog_head, // 9
    sub_prog_head, // 10

    // parameter
    formal_para, // 11
    formal_para, // 12
    formal_para, // 13
    para_list, // 14
    para_list, // 15
    para, // 16
    para, // 17
    var_para, // 18
    value_para, // 19

    // const
    const_defs, // 20
    const_defs, // 21
    const_def, // 22
    const_def, // 23

    cont, // 24
    cont, // 25
    cont, // 26
    cont, // 27

    // var
    var_defs, // 28
    var_defs, // 29
    var_def, // 30
    var_def, // 31
    var_with_type, // 32
    var_with_type, // 33

    // type
    type, // 34
    type, // 35
    type_base, // 36
    type_base, // 37
    type_base, //  38
    type_base, // 39
    period_with_type, // 40
    period_with_type, // 41

    // id 42-47
    id_with_type, // 42
    id_with_type, // 43
    var, // 44
    var, // 45
    var_list, // 46
    var_list, // 47

    // op  48-60
    op_cmp, // 48
    op_cmp, // 49
    op_cmp, // 50
    op_cmp, // 51
    op_cmp, // 52
    op_cmp, // 53

    op_add_sub, // 54
    op_add_sub, // 55
    op_add_sub, // 56

    op_div_mul, // 57
    op_div_mul, // 58
    op_div_mul, // 59
    op_div_mul, // 60

    // statement 61-80
    main, // 61
    begin, // 62
    stmt_list, // 63
    stmt_list, // 64

    stmt, // 65
    stmt, // 66

    stmt_base, // 67
    stmt_base, // 68
    stmt_base, // 69
    stmt_base, // 70
    stmt_base, // 71
    stmt_base, // 72
    stmt_base, // 73
    stmt_base, // 74
    stmt_base, // 75


    proc_call, // 76
    proc_call, // 77
    proc_call, // 78

    else_part, // 79
    else_part, // 80

    // expression 81-84
    exp_list, // 81
    exp_list, // 82

    TokenState::exp, // 83
    TokenState::exp, // 84


    // sub_exp
    sub_exp, // 85
    sub_exp, // 86


    // term
    term, // 87
    term, // 88


    // factor 89-97
    factor, // 89
    factor, // 90
    factor, // 91
    factor, // 92
    factor, // 93
    factor, // 94
    factor, // 95
    factor, //  96
    factor, // 97

    //
    then, // 98
    els, // 99
    doo, // 100
    to, // 101
    iff, // 102
    whl, // 103
    fore, // 104
    end, // 105
    todo, // 106

    factor, // 107
    cont, // 108
    boolean, // 109
    boolean, // 110

    ext_sub_prog_head,    // 111
    ext_sub_prog_head,  // 112
    sub_prog   // 113

};

static std::vector<Token> rhs[] = {

    // program
    {prog, real_end},  // 0
    {prog_head, p_semicolon, prog_body, p_dot},  // 1
    {key_prog, id},  // 2
    {key_prog, id, p_l_paren, id, p_r_paren},  // 3
    {const_defs, var_defs, sub_prog_def, main},   // 4

    // sub program 5-10
    {},
    {sub_prog, p_semicolon, sub_prog_def},
    {sub_prog_head, p_semicolon, sub_prog_body},
    {const_defs, var_defs, main}, // 8
    {key_proc, idf, formal_para}, // 9
    { key_func, idf, formal_para, p_colon, type_base},  // 10

    // parameter
    {},
    {p_l_paren, para_list, p_r_paren},
    {p_l_paren, p_r_paren},
    {para},
    {para, p_semicolon, para_list},
    {var_para},
    {value_para},
    {key_var, value_para},
    {id_with_type},

    // const  20-27
    {},
    {key_const, const_def, p_semicolon},
    {id, op_equal, cont},
    {const_def, p_semicolon, id, op_equal, cont},

    {letter}, // 24
    {op_neg, num}, // 25
    {op_pos, num}, // 26
    {num}, // 27

    // var 28-33
    {},
    {key_var, var_def, p_semicolon},
    {var_with_type},
    {var_def, p_semicolon, var_with_type},
    {id, p_comma, var_with_type},
    {id, p_colon, type},

    // type  34-41
    {type_base},
    {type_array, op_l_squ, period_with_type}, // 35
    {type_int},
    {type_real},
    {type_char},
    {type_bool},
    {digit, p_dotdot, digit, p_comma, period_with_type},
    {digit, p_dotdot, digit, op_r_squ, key_of, type_base},

    // id 42-47
    {id, p_colon, type_base},
    {id, p_comma, id_with_type},
    {id}, // 44
    {id, op_l_squ, exp_list, op_r_squ}, // 45
    {var}, // 46
    {var_list, p_comma, var}, // 47

    // op 48-60
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

    // statement 61-80
    {begin, stmt_list, end},
    {key_begin}, // 62
    {stmt},
    {stmt_list, p_semicolon, stmt},

    {},
    {stmt_base},

    {main}, // 67
    {proc_call}, // 68
    {var, op_assign, TokenState::exp}, // 69
    {idf, op_assign, TokenState::exp}, // 70
    {key_read, p_l_paren, var_list, p_r_paren}, // 71
    {key_write, p_l_paren, exp_list, p_r_paren}, // 72
    {fore, var, op_assign, TokenState::exp, to, TokenState::exp, todo, stmt_base},
    {whl, TokenState::exp, doo, stmt_base}, // 74
    {iff, TokenState::exp, then, stmt, else_part}, // 75

    {idf}, // 76
    {idf, p_l_paren, exp_list, p_r_paren}, // 77
    {idf, p_l_paren, p_r_paren}, // 78

    {}, // 79
    {els, stmt_base}, // 80

    // exp 81-84
    {TokenState::exp}, // 81
    {exp_list, p_comma, TokenState::exp}, // 82

    {sub_exp}, // 83
    {TokenState::exp, op_cmp, sub_exp},


    // sub_exp  85-86
    {term}, // 85
    {sub_exp, op_add_sub, term},


    // term  87-88
    {factor}, // 87
    {term, op_div_mul, factor},


    // factor  89-97
    {num}, // 89
    {var}, // 90
    {p_l_paren, TokenState::exp, p_r_paren}, // 91
    {idf, p_l_paren, exp_list, p_r_paren}, // 92
    {idf, p_l_paren, p_r_paren}, // 93
    {idf}, // 94

    {op_not, factor},
    {op_neg, factor},
    {op_pos, factor},

    {key_then}, // 98
    {key_else}, // 99
    {key_do}, // 100
    {key_to}, // 101
    {key_if}, // 102
    {key_while}, // 103
    {key_for}, // 104
    {key_end}, // 105
    {key_do}, // 106

    {boolean}, // 107
    {boolean}, // 108
    {truly}, // 109
    {falsely}, // 110

    {key_external, key_proc, idf, formal_para}, // 111
    { key_external, key_func, idf, formal_para, p_colon, type_base},  // 112
    {ext_sub_prog_head}, // 113

};

static_assert(sizeof(lhs) / sizeof(Token) == sizeof(rhs) / sizeof(std::vector<Token>));

const int entryCount = sizeof(lhs) / sizeof(Token);

inline SyntaxArray& syntax::getSyntaxes()
{
    static SyntaxArray syntaxes;
    return syntaxes;
}

syntax::SyntaxEntry::SyntaxEntry(size_t id) :
    id(id),
    l(lhs[id]),
    r(rhs[id])
{
}

void syntax::initSyntaxes()
{
    for (int i = 0; i < entryCount; ++i)
        getSyntaxes().emplace_back(i);
}
