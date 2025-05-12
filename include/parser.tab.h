#ifndef YY_YY_MNT_C_USERS_LENOVO_DESKTOP_1_PASCC_MAIN_BUILD_INCLUDE_PARSER_TAB_H_INCLUDED
# define YY_YY_MNT_C_USERS_LENOVO_DESKTOP_1_PASCC_MAIN_BUILD_INCLUDE_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PROGRAM = 258,                 /* PROGRAM  */
    FUNCTION = 259,                /* FUNCTION  */
    PROCEDURE = 260,               /* PROCEDURE  */
    TO = 261,                      /* TO  */
    DOWNTO = 262,                  /* DOWNTO  */
    ARRAY = 263,                   /* ARRAY  */
    TYPE = 264,                    /* TYPE  */
    CONST = 265,                   /* CONST  */
    RECORD = 266,                  /* RECORD  */
    IF = 267,                      /* IF  */
    THEN = 268,                    /* THEN  */
    ELSE = 269,                    /* ELSE  */
    CASE = 270,                    /* CASE  */
    OF = 271,                      /* OF  */
    WHILE = 272,                   /* WHILE  */
    DO = 273,                      /* DO  */
    FOR = 274,                     /* FOR  */
    REPEAT = 275,                  /* REPEAT  */
    UNTIL = 276,                   /* UNTIL  */
    BEGIN_ = 277,                  /* BEGIN_  */
    END = 278,                     /* END  */
    ADDOP = 279,                   /* ADDOP  */
    NOT = 280,                     /* NOT  */
    PLUS = 281,                    /* PLUS  */
    UMINUS = 282,                  /* UMINUS  */
    CONSTASSIGNOP = 283,           /* CONSTASSIGNOP  */
    ID = 284,                      /* ID  */
    CHAR = 285,                    /* CHAR  */
    INT_NUM = 286,                 /* INT_NUM  */
    REAL_NUM = 287,                /* REAL_NUM  */
    BASIC_TYPE = 288,              /* BASIC_TYPE  */
    RELOP = 289,                   /* RELOP  */
    MULOP = 290,                   /* MULOP  */
    STRING_ = 291,                 /* STRING_  */
    VAR = 292,                     /* VAR  */
    SUBCATALOG = 293,              /* SUBCATALOG  */
    ASSIGNOP = 294,                /* ASSIGNOP  */
    WRITE = 295,                   /* WRITE  */
    WRITELN = 296,                 /* WRITELN  */
    SEP = 297,                     /* SEP  */
    READ = 298,                    /* READ  */
    READLN = 299,                  /* READLN  */
    TRUE = 300,                    /* TRUE  */
    FALSE = 301                    /* FALSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 44 "/mnt/c/Users/Lenovo/Desktop/1/PASCC-main/build/../scripts/parser.y"

#line 112 "/mnt/c/Users/Lenovo/Desktop/1/PASCC-main/build/../include/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (pascals::ast::AST *real_ast);


#endif /* !YY_YY_MNT_C_USERS_LENOVO_DESKTOP_1_PASCC_MAIN_BUILD_INCLUDE_PARSER_TAB_H_INCLUDED  */
