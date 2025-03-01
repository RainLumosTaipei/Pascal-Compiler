#pragma once

#include <string>
#include <iostream>

namespace token {

    enum TokenState {

        // non terminal
        
        real_start,             // S
        
        prog,
        prog_head,
        prog_body,
        
        var_body,               // var body
        var_def,                // var declare
        const_body,             // const body
        const_def,              // const declare
        
        main,                   // main body
        begin,                  // begin -> key_begin
        stmt,                   // statement
        stmt_list,              // statement list
        exp,                    // expression
        subexp,                 // simple exp
        term,                   // term exp
        factor,                 // factor exp

        variable,               // variable
        cont,                   // const

        type_base,              // basic type
        type,                   // type
        ids,                    // id list

        // terminal
        // type
        type_int,               // int
        type_real,              // real
        type_char,              // char
        type_array,             // array
        type_bool,              // boolean
        // value
        id,                     // identifier
        integer,                // int num
        real,                   // float num
        character,              // char
        // punctuation
        p_l_bracket,            // {
        p_r_bracket,            // }
        p_l_paren,              // (
        p_r_paren,              // )
        p_colon,                // :
        p_quote,                // '
        p_comma,                // ,
        p_semicolon,            // ;
        p_dotdot,               // ..
        p_dot,                  // .
        // operator
        op_equal,               // =
        op_add,                 // +
        op_minus,               // -
        op_mul,                 // *
        op_div,                 // div
        op_not_equal,           // <>
        op_l_square,            // [
        op_r_square,            // ]
        op_assign,              // :=
        op_mod,                 // mod
        op_less,                // <
        op_less_equal,          // <=
        op_great,               // >
        op_great_equal,         // >=
        op_and,                 // and
        op_or,                  // or
        op_not,                 // not
        // keyword
        key_for,                // for
        key_if,                 // if
        key_else,               // else
        key_then,               // then
        key_to,                 // to
        key_do,                 // do
        key_of,                 // of
        key_const,              // const
        key_var,                // var
        key_begin,              // begin
        key_end,                // end
        key_prog,               // program
        key_proc,               // procedure
        key_func,               // function
        key_read,               // read
        key_write,              // write
        // other
        null,                   // null
        real_end                // $
    } ;

    constexpr int nonTerminalCount = TokenState::type_int;
    constexpr int tokenCount = TokenState::real_end + 1;

    extern std::string tokenNames[];

    constexpr void validateTerminal(int t) {
        if (t < nonTerminalCount) {
            throw std::invalid_argument("Token is not terminal.");
        }
    }

    constexpr void validateNonTerminal(int t) {
        if (t >= nonTerminalCount) {
            throw std::invalid_argument("Token is not nonterminal.");
        }
    }

    
    struct Token {

        TokenState token;
        
        explicit Token(int t) : token(static_cast<TokenState>(t)){}

        explicit Token(size_t t) : token(static_cast<TokenState>(t)){}

        Token(TokenState t) : token(t) {}
        
        operator TokenState() const {
            return token;
        }

        operator int() const {
            return (int) token;
        }

        operator size_t() const {
            return (size_t) token;
        }

        inline bool operator==(const Token &other) const {
            return token == other.token;
        }

        inline bool operator < (const Token &other) const {
            return token < other.token;
        }

        inline bool isTerminal() const {
            return token >= nonTerminalCount;
        }

        inline bool isNull() const {
            return token == TokenState::null;
        }
    };

    struct TermToken : public Token
    {
        TermToken(TokenState t) : Token(t)
        {
            validateTerminal(t);
        }

        template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
        TermToken(T t) : Token(static_cast<int>(t)) {
            validateTerminal(static_cast<int>(t));
        }
    };

    struct NonTermToken : public Token
    {
        NonTermToken(TokenState t) : Token(t)
        {
            validateNonTerminal(t);
        }

        template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
        NonTermToken(T t) : Token(static_cast<int>(t)) {
            validateNonTerminal(static_cast<int>(t));
        }
    };

    inline std::ostream &operator<<(std::ostream &os, const Token &s) {
        return os << tokenNames[s.token];
    }
    

}



