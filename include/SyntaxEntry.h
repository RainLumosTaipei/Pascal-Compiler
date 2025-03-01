#pragma once
#include <vector>

#include "Token.h"

namespace syntax
{

    struct SyntaxEntry
    {
        size_t id;
        token::Token l;
        std::vector<token::Token> r;
        
        explicit SyntaxEntry(size_t id);
        
        inline bool operator==(const SyntaxEntry &other) const {
            return id == other.id;
        }

        inline bool operator<(const SyntaxEntry &other) const {
            return id < other.id;
        }

        inline bool isNull() const
        {
            return r.empty();
        }
    };

    inline std::ostream &operator<<(std::ostream &os, const SyntaxEntry &s) {
        os << s.l << " -> ";
        for (auto token : s.r)
            os << token << " ";
        return os;
    }

    typedef std::vector<SyntaxEntry> SyntaxArray;

    SyntaxArray& getSyntaxes();
    void initSyntaxes();
}

