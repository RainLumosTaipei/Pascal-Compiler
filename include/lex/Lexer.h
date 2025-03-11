#pragma once

#include <deque>
#include <unordered_map>

#include "Token.h"

namespace token
{
    struct TokenDesc;

    enum LexerState{
        normal,
        array,
        func
    };
    
    class Lexer
    {
    public:
        static size_t line;
        static size_t col;
        static LexerState state;
        static TokenState prev;
        
        Lexer(std::string input):input_(input), pos_(0) {}
        
        TokenDesc* getNextToken();
        
    private:
        std::string input_;
        size_t pos_;

        void skip();
        bool skipComment();

        TokenDesc* idOrKey();
        TokenDesc* num();
        TokenDesc* str();
        TokenDesc* punc();

        TokenState key(const std::string& value);
        TokenState singlePunc(char value);
        TokenState doublePunc(const std::string& value);
    };

    struct TokenDesc {
        
        size_t line;
        size_t col;
        
        Token token;
        std::string value;
        
        TokenDesc(Token token, std::string value) :
                line(Lexer::line),col(Lexer::col),
                token(token),value(std::move(value)){}

        explicit TokenDesc(Token t):  line(0), col(0), token(t), value("undefined") {}

        TokenDesc(Token t, const TokenDesc* desc):  line(0), col(0), token(t), value(desc->value) {}
    };

    inline std::ostream &operator<<(std::ostream &os, const TokenDesc &s) {
        return os << s.token;
    }

    typedef std::deque<TokenDesc*> TokenDeque;


    typedef std::unordered_map<std::string, TokenState> StrMap;
    typedef std::unordered_map<char, TokenState> CharMap;


    TokenDeque& getTokens();
    void lex(const std::string& filename);
    void lex();
    void printTokens();
}
