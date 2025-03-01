﻿#include "Lexer.h"

#include <unordered_map>

using namespace token;
using namespace std;

size_t token::Lexer::col = 1 ;
size_t token::Lexer::line = 1 ;

static const CharMap puncMap = {
        {'{', TokenState::p_l_bracket},
        {'}', TokenState::p_r_bracket},
        {'(', TokenState::p_l_paren},
        {')', TokenState::p_r_paren},
        {',', TokenState::p_comma},
        {':', TokenState::p_colon},
        {';', TokenState::p_semicolon},
        {'.', TokenState::p_dot},
        {'[', TokenState::op_l_square},
        {']', TokenState::op_r_square},
        {'<', TokenState::op_less},
        {'>', TokenState::op_great},
        {'+', TokenState::op_add},
        {'-', TokenState::op_sub},
        {'*', TokenState::op_mul},
        {'/', TokenState::op_div},
        {'=', TokenState::op_equal}
};

static const StrMap keyMap = {
        {"for", TokenState::key_for},
        {"if", TokenState::key_if},
        {"else", TokenState::key_else},
        {"then", TokenState::key_then},
        {"to", TokenState::key_to},
        {"do", TokenState::key_do},
        {"of", TokenState::key_of},
        {"var", TokenState::key_var},
        {"const", TokenState::key_const},
        {"begin", TokenState::key_begin},
        {"end", TokenState::key_end},
        {"program", TokenState::key_prog},
        {"procedure", TokenState::key_proc},
        {"function", TokenState::key_func},
        {"read", TokenState::key_read},
        {"write", TokenState::key_write},
        {"integer", TokenState::type_int},
        {"real", TokenState::type_real},
        {"char", TokenState::type_char},
        {"array", TokenState::type_array},
        {"boolean", TokenState::type_bool},
        {"and", TokenState::op_and},
        {"not", TokenState::op_not},
        {"div", TokenState::op_div},
        {"mod", TokenState::op_mod},
        {"or", TokenState::op_or}
};


void Lexer::skip() {
    while (pos_ < input_.size() && (std::isspace(input_[pos_]) || input_[pos_] == '\n')) {
        if (input_[pos_] == '\n') {
            Lexer::line++;
            Lexer::col = 1;
        } else {
            Lexer::col++;
        }
        pos_++;
    }
}

TokenDesc* Lexer::getNextToken() {
    skip();

    if (pos_ >= input_.size()) {
        return new TokenDesc(TokenState::real_end, "");
    }

    char ch = input_[pos_];

    if (isalpha(ch) || ch == '_') {
        return idOrKey();
    }
    else if (isdigit(ch)) {
        return num();
    }
    else if (ch == '\'') {
        return str();
    }

    return punc();
}

TokenDesc* Lexer::idOrKey(){
    size_t start = pos_;
    while (pos_ < input_.size() && ( isalnum(input_[pos_]) || input_[pos_] == '_')) {
        pos_++;
    }
    string value = input_.substr(start, pos_ - start);
    Lexer::col += value.length();
    return new TokenDesc(key(value), value);
}

TokenState Lexer::key(const std::string& value) {
    auto it = keyMap.find(value);
    if (it != keyMap.end()) {
        return it->second;
    }
    return TokenState::id;
}

// TODO: consider real number
TokenDesc* Lexer::num() {
    size_t start = pos_;
    while (pos_ < input_.size() && isdigit(input_[pos_])) {
        pos_++;
    }
    string value = input_.substr(start, pos_ - start);
    Lexer::col += value.length();
    return new TokenDesc(TokenState::num, value);
}

TokenDesc* Lexer::str() {
    pos_++; // 跳过开头的引号
    size_t start = pos_;
    while (pos_ < input_.size() && input_[pos_] != '\'') {
        pos_++;
    }
    string value = input_.substr(start, pos_ - start);
    pos_++; // 跳过结尾的双引号
    Lexer::col += value.length() + 2;
    return new TokenDesc(TokenState::letter, value);
}

TokenDesc* Lexer::punc() {

    string value = input_.substr(pos_, 2);
    TokenState t = doublePunc(value);
    if (t != TokenState::null)
    {
        pos_ += 2;
        Lexer::col+= 2;
        return new TokenDesc(t, value);
    }

    char ch = input_[pos_];
    t = singlePunc(ch);
    pos_++;
    Lexer::col++;
    return new TokenDesc(t, value);
}

TokenState Lexer::doublePunc(const std::string& value) {
    if (value == "<>") {
        return TokenState::op_not_equal;
    } else if (value == "<=") {
        return TokenState::op_less_equal;
    } else if (value == ">=") {
        return TokenState::op_great_equal;
    } else if (value == ":=") {
        return TokenState::op_assign;
    } else if (value == "..") {
        return TokenState::p_dotdot;
    } else {
        return TokenState::null;
    }
}

TokenState Lexer::singlePunc(char value) {
    auto it = puncMap.find(value);
    if (it != puncMap.end()) {
        return it->second;
    }
    return TokenState::null;
}



