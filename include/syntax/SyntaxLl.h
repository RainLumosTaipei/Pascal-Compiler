#pragma once
#include <set>
#include <unordered_map>
#include <vector>

#include "SyntaxEntry.h"

namespace syntax::ll
{
    struct FirstToken
    {
        token::TermToken token;
        int id;

        FirstToken(int t): token(t), id(0)
        {
        }

        inline bool operator==(const FirstToken& other) const
        {
            return token == other.token;
        }

        inline bool operator<(const FirstToken& other) const
        {
            return token < other.token;
        }
    };

    struct FirstEntry
    {
        std::set<FirstToken> set;
        bool canNull;

        FirstEntry() : canNull(false)
        {
        }

        size_t merge(const FirstEntry& entry);

        bool find(token::Token t);
    };

    struct FollowEntry
    {
        std::set<FirstToken> set;
    };

    using FirstArray = std::vector<FirstEntry>;
    using FollowArray = FirstArray;
    using FirstTable = std::unordered_map<token::NonTermToken, std::unordered_map<token::TermToken, int>>;

    FirstArray& getFirstArray();
    FollowArray& getFollowArray();

    void searchNull();
    void searchFirst();
    void searchFollow();

    size_t mergeFirst(const SyntaxEntry& syntax, size_t loc);

    void initFirst();
    void printFirst();
    void printFollow();
}
