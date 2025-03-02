# 文法生成式

## program

real_start -> prog real_end

prog -> prog_head ; prog_body .

prog_head -> program id 

prog_head -> program id ( ids )

prog_body -> const_defs var_defs sub_prog_def main

## sub program

sub_prog_def -> null

sub_prog_def -> sub_prog_def sub_prog ;

sub_prog -> sub_prog_head ; sub_prog_body

sub_prog_body -> const_defs var_defs main

sub_prog_head -> key_proc id formal_para

sub_prog_head -> key_func id formal_para : type_base

## parameter

formal_para -> null

formal_para -> ( para_list )

para_list -> para

para_list -> para_list ; para

para -> var_para

para -> value_para

var_para -> key_var value_para

value_para -> ids : type_base

## const

const_defs -> null

const_defs -> const const_def

const_def -> id = const ;

const_def -> const_def id = const ;

const -> 'char'

const -> + num

const -> - num

const -> num


## var

var_defs -> null

var_defs -> var var_def ;

var_def -> ids : type 

var_def -> var_def ; ids : type 

## type

type -> type_base

type -> key_array [ period ] key_of type_base

type_base -> integer

type_base -> real

type_base -> char

type_base -> boolean

period -> digit .. digit

period -> period , digit .. digit


## id

ids -> id

ids -> ids , id

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

stmt_list -> stmt_list ; stmt 

stmt -> null

stmt -> main

stmt -> proc_call

stmt -> var := exp 

stmt -> idf := exp 

stmt -> read ( var_list )

stmt -> write ( exp_list )

stmt -> for id := exp to exp do stmt

stmt -> if exp then stmt else_part

proc_call -> id

proc_call -> id ( exp_list )

else_part -> null

else_part -> else stmt

## exp

exp_list -> exp

exp_list -> exp_list , exp

exp -> sub_exp

exp -> sub_exp op_cmp sub_exp


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

factor -> id ( exp_list )

factor -> not factor

factor -> op_neg factor

