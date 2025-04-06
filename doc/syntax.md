# 文法生成式

## program

real_start -> prog real_end

prog -> prog_head ; prog_body .

prog_head -> program id 

prog_head -> program id ( id )

prog_body -> const_defs var_defs sub_prog_def main

## const

const_defs -> null

const_defs -> const const_def ;

const_def -> id = const

const_def -> const_def ; id = const 

const -> char

const -> op_pos num

const -> op_neg num

const -> num

## sub program

sub_prog_def -> null

sub_prog_def -> sub_prog ; sub_prog_def

sub_prog -> sub_prog_head ; sub_prog_body

sub_prog_body -> const_defs var_defs main

sub_prog_head -> key_proc idf formal_para

sub_prog_head -> key_func idf formal_para : type_base

## parameter

formal_para -> null

formal_para -> ( para_list )

formal_para -> ( )

para_list -> para

para_list -> para ; para_list

para -> var_para

para -> value_para

var_para -> key_var value_para

value_para -> id_with_type

## var_def

var_defs -> null

var_defs -> var var_def ;

var_def -> var_with_type

var_def -> var_def ; var_with_type

var_with_type -> id : type

var_with_type -> id , var_with_type

## type

type -> type_base

type -> key_array [ period_with_type

type_base -> integer

type_base -> real

type_base -> char

type_base -> boolean

period_with_type -> digit .. digit , period_with_type

period_with_type -> digit .. digit ] key_of type_base

## id (left value)

id_with_type -> id : type_base

id_with_type -> id , id_with_type

var -> id

var -> id [ exp_list ]

var_list -> var

var_list -> var_list , var

## op

op_cmp -> >

op_cmp -> < 

op_cmp -> =

op_cmp -> >=

op_cmp -> <=

op_cmp -> <>

op_add_sub -> +

op_add_sub -> -

op_add_sub -> or

op_div_mul -> div

op_div_mul -> *

op_div_mul -> mod

op_div_mul -> and

## statement

main -> begin stmt_list end

begin -> key_begin

stmt_list -> stmt 

stmt_list -> stmt_list  ; stmt

stmt -> null

stmt -> stmt_base

stmt_base -> main

stmt_base -> proc_call

stmt_base -> var := exp 

stmt_base -> idf := exp 

stmt_base -> read ( var_list )

stmt_base -> write ( exp_list )

stmt_base -> for var := exp to exp do stmt_base

stmt_base -> while exp do stmt_base

stmt_base -> if exp then stmt else_part

proc_call -> idf

proc_call -> idf ( exp_list )

proc_call -> idf (  )

else_part -> null

else_part -> else stmt_base

## exp

exp_list -> exp

exp_list -> exp_list , exp

exp -> sub_exp

exp -> exp op_cmp sub_exp



## sub_exp

sub_exp -> term

sub_exp -> sub_exp op_add_sub term



## term

term -> factor

term -> term op_div_mul factor



## factor

factor -> num

factor -> var

factor -> ( exp )

factor -> idf ( exp_list )

factor -> idf (  )

factor -> idf

factor -> not factor

factor -> op_neg factor

factor -> op_pos factor

factor -> boolean

const -> boolean

boolean -> true

boolean -> false