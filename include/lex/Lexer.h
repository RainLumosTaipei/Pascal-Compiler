#pragma once

#include <deque>
#include <unordered_map>


#include "Token.h"


namespace token
{
    struct TokenDesc;

    enum LexerState
    {
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

        Lexer(std::string input): input_(input), pos_(0)
        {
        }

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

    struct TokenDesc
    {
        size_t line;
        size_t col;


        std::string str;

        Token token;


        explicit TokenDesc(Token t): line(0), col(0), token(t)
        {
        }

        TokenDesc(Token token, std::string str) :
            line(Lexer::line), col(Lexer::col),
            str(std::move(str)), token(token)
        {
        }

        TokenDesc(const TokenDesc& desc) = default;

        TokenDesc& operator=(const TokenDesc& desc)
        {
            col = desc.col;
            line = desc.line;
            str = desc.str;
            return *this;
        }
    };

    inline std::ostream& operator<<(std::ostream& os, const TokenDesc& s)
    {
        return os << s.token;
    }

    using TokenDeque = std::deque<TokenDesc*>;


    using StrMap = std::unordered_map<std::string, TokenState>;
    using CharMap = std::unordered_map<char, TokenState>;


    TokenDeque& getTokens();
    void lex(const std::string& filename);
    void lex();
    void printTokens();
}
