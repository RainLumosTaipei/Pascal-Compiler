#include "lex/Lexer.h"

#include <unordered_map>
#include <vector>
#include <set>

#include "lex/LexErr.h"

using namespace token;
using namespace std;

size_t Lexer::col = 1;
size_t Lexer::line = 1;
LexerState Lexer::state = normal;
TokenState Lexer::prev = null;
std::string Lexer::filename = " ";

static const CharMap puncMap = {
    {'{', p_l_bracket},
    {'}', p_r_bracket},
    {'(', p_l_paren},
    {')', p_r_paren},
    {',', p_comma},
    {':', p_colon},
    {';', p_semicolon},
    {'.', p_dot},
    {'[', op_l_squ},
    {']', op_r_squ},
    {'<', op_less},
    {'>', op_great},
    {'+', op_add},
    {'-', op_sub},
    {'*', op_mul},
    {'/', op_div},
    {'=', op_equal}
};

static const std::set<TokenState> opMap = {
    op_less,
    op_great,
    op_add,
    op_sub,
    op_mul,
    op_div,
    op_equal,
    op_not_equ,
    op_assign,
    op_great_equ,
    op_less_equ,
    p_l_paren,
    op_neg,
    op_pos
};

static std::set<std::string> idfMap;

static const StrMap keyMap = {
    {"for", key_for},
    {"if", key_if},
    {"while", key_while},
    {"else", key_else},
    {"then", key_then},
    {"to", key_to},
    {"do", key_do},
    {"of", key_of},
    {"var", key_var},
    {"const", key_const},
    {"begin", key_begin},
    {"end", key_end},
    {"program", key_prog},
    {"procedure", key_proc},
    {"function", key_func},
    {"read", key_read},
    {"write", key_write},
    {"integer", type_int},
    {"real", type_real},
    {"char", type_char},
    {"array", type_array},
    {"boolean", type_bool},
    {"and", op_and},
    {"not", op_not},
    {"div", op_div},
    {"mod", op_mod},
    {"or", op_or},
    {"true", truly},
    {"false", falsely},
    {"external", key_external}
};


void Lexer::skip()
{
    // erase UTF-8 head
    while (input_[pos_] < 0) ++pos_;
    while (pos_ < input_.size() && (std::isspace(input_[pos_]) || input_[pos_] == '\n'))
    {
        if (input_[pos_] == '\n')
        {
            ++line;
            col = 1;
        }
        else
        {
            ++col;
        }
        ++pos_;
    }
}

bool Lexer::skipComment()
{
    if ('{' == input_[pos_])
    {
        while ('}' != input_[pos_])
        {
            ++pos_;
            ++col;
        }
        ++pos_;
        ++col;
        return true;
    }

    if (pos_ < input_.size() - 1 && input_[pos_] == '/' && input_[pos_ + 1] == '/')
    {
        while ('\n' != input_[pos_])
        {
            ++pos_;
        }
        ++pos_;
        ++line;
        col = 1;
        return true;
    }
    return false;
}

TokenDesc* Lexer::getNextToken()
{
start:
    skip();

    if (pos_ >= input_.size())
    {
        return new TokenDesc(real_end, "");
    }

    if (skipComment()) goto start;

    char ch = input_[pos_];

    if (isalpha(ch) || ch == '_')
    {
        return idOrKey();
    }
    if (isdigit(ch))
    {
        return num();
    }
    if (ch == '\'')
    {
        return str();
    }

    return punc();
}

TokenDesc* Lexer::idOrKey()
{
    size_t start = pos_;
    while (pos_ < input_.size() && (isalnum(input_[pos_]) || input_[pos_] == '_'))
    {
        pos_++;
    }
    string value = input_.substr(start, pos_ - start);
    col += value.length();
    return new TokenDesc(key(value), value);
}

TokenState Lexer::key(const std::string& value)
{
    auto it = keyMap.find(value);
    if (it != keyMap.end())
    {
        // array
        if (value == "array")
            state = array;
        prev = it->second;
        return it->second;
    }
    if (prev == key_func || prev == key_proc)
        idfMap.insert(value);
    if (idfMap.count(value))
    {
        prev = idf;
        return idf;
    }
    prev = id;
    return id;
}


TokenDesc* Lexer::num()
{
    size_t start = pos_;
    while (pos_ < input_.size() && isdigit(input_[pos_]))
    {
        ++pos_;
    }
    // real 10.23
    if (state != array && input_[pos_] == '.')
    {
        ++pos_;
        while (pos_ < input_.size() && isdigit(input_[pos_]))
        {
            ++pos_;
        }
    }
    string value = input_.substr(start, pos_ - start);
    col += value.length();

    // array 需要使用 digit
    if (state == array)
    {
        prev = digit;
        return new TokenDesc(digit, value);
    }

    prev = TokenState::num;
    return new TokenDesc(TokenState::num, value);
}

TokenDesc* Lexer::str()
{
    pos_++; // 跳过开头的引号
    size_t start = pos_;
    while (pos_ < input_.size() && input_[pos_] != '\'')
    {
        pos_++;
    }
    string value = input_.substr(start, pos_ - start);
    pos_++; // 跳过结尾的双引号
    col += value.length() + 2;
    prev = letter;
    return new TokenDesc(letter, value);
}

TokenDesc* Lexer::punc()
{
    string value = input_.substr(pos_, 2);
    TokenState t = doublePunc(value);
    if (t != null)
    {
        pos_ += 2;
        col += 2;
        prev = t;
        return new TokenDesc(t, value);
    }

    char ch = input_[pos_];
    t = singlePunc(ch);
    pos_++;
    col++;
    // array
    if (t == op_r_squ && state == array)
        state = normal;
    // negative
    if (t == op_sub && opMap.count(prev))
        t = op_neg;
    // positive
    if (t == op_add && opMap.count(prev))
        t = op_pos;
    prev = t;
    return new TokenDesc(t, value);
}

TokenState Lexer::doublePunc(const std::string& value)
{
    if (value == "<>")
    {
        prev = op_not_equ;
        return op_not_equ;
    }
    if (value == "<=")
    {
        prev = op_less_equ;
        return op_less_equ;
    }
    if (value == ">=")
    {
        prev = op_great_equ;
        return op_great_equ;
    }
    if (value == ":=")
    {
        prev = op_assign;
        return op_assign;
    }
    if (value == "..")
    {
        prev = p_dotdot;
        return p_dotdot;
    }

    return null;
}

TokenState Lexer::singlePunc(char value)
{
    auto it = puncMap.find(value);
    if (it != puncMap.end())
    {
        return it->second;
    }
    throw LexErr("unknown punctuation", string(1, value));
}
