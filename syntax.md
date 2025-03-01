#

real_start -> prog real_end

prog -> prog_head prog_body
prog_head -> program id ;
prog_body -> const_body var_body main

const_body -> null
const_body -> const const_def
const_def -> id = const ;
const_def -> const_def id = const ;

var_body -> null
var_body -> var var_def
var_def -> ids : type ;
var_def -> var_def ids : type ;

type -> type_base
type_base -> integer
type_base -> real
type_base -> char
type_base -> boolean

ids -> id
ids -> ids , id

main -> begin stmt_list end .
begin -> begin
stmt_list -> stmt
stmt_list -> stmt_list stmt

stmt -> variable := 1 + 2 ;
variable -> id
variable -> id[exp]
const -> 'char'
const -> + int
const -> - int
const -> int
const -> + real
const -> - real
const -> real

exp -> subexp
exp -> subexp > subexp
exp -> subexp < subexp
exp -> subexp = subexp
exp -> subexp >= subexp
exp -> subexp <= subexp
exp -> subexp <> subexp

subexp -> term
subexp -> term + term
subexp -> term - term
subexp -> term or term

term -> factor
factor -> factor * factor
factor -> factor div factor
factor -> factor mod factor
factor -> factor and factor

factor -> not factor
factor -> - factor
factor -> ( exp )
factor -> variable
factor -> const