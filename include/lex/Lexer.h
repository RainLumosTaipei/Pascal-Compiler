#pragma once

#include <deque>
#include <unordered_map>
#include <llvm/IR/Value.h>

#include "Token.h"


namespace semantic
{
    struct SymbolEntry {
        // 保存变量的地址，而不是右值
        llvm::Value* val;
        llvm::Type* type;

        SymbolEntry() = default;
        SymbolEntry(const SymbolEntry& desc) = default;
        SymbolEntry& operator=(const SymbolEntry& entry) = default;
    };
}

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
        
        semantic::SymbolEntry entry;
        std::string str;
        
        Token token;
        Token save;

        explicit TokenDesc(Token t): line(0), col(0),entry(),token(t), save(null) {}
        
        TokenDesc(Token token, std::string str) :
                line(Lexer::line),col(Lexer::col),entry(),
                str(std::move(str)), token(token),save(null){}
        
        TokenDesc(const TokenDesc& desc) = default;
        TokenDesc& operator=(const TokenDesc& desc)
        {
            col = desc.col;
            line = desc.line;
            entry = desc.entry;
            str = desc.str;
            save = desc.save;
            return *this;
        }
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
