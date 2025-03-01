# 文法生成式

## start

real_start -> prog real_end

prog -> prog_head prog_body

prog_head -> program id ;

prog_body -> const_body var_body main

## const

const_body -> null

const_body -> const const_def

const_def -> id = const ;

const_def -> const_def id = const ;

const -> 'char'

const -> + num

const -> - num

const -> num


## var

var_body -> null

var_body -> var var_def

var_def -> ids : type ;

var_def -> var_def ids : type ;

## type

type -> type_base

type_base -> integer

type_base -> real

type_base -> char

type_base -> boolean

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

main -> begin stmt_list end .

begin -> key_begin

stmt_list -> stmt 

stmt_list -> stmt_list ; stmt 

stmt -> null

stmt -> main

stmt -> var := exp 

stmt -> read ( var_list )

stmt -> write ( exp_list )

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

factor -> - factor

