#pragma once
#include <vector>

#include "lex/Token.h"

namespace syntax
{
    struct SyntaxEntry
    {
        size_t id;
        token::Token l;
        std::vector<token::Token> r;

        explicit SyntaxEntry(size_t id);

        inline bool operator==(const SyntaxEntry& other) const
        {
            return id == other.id;
        }

        inline bool operator<(const SyntaxEntry& other) const
        {
            return id < other.id;
        }

        inline bool isNull() const
        {
            return r.empty();
        }
    };

    inline std::ostream& operator<<(std::ostream& os, const SyntaxEntry& s)
    {
        for (auto token : s.r)
            os << token << " ";
        if (s.isNull()) os << "null ";
        os << "-> " << s.l;
        return os;
    }

    using SyntaxArray = std::vector<SyntaxEntry>;

    SyntaxArray& getSyntaxes();
    void initSyntaxes();
}
