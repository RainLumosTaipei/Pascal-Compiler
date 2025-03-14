#pragma once

#include <set>
#include <unordered_map>

#include "SyntaxEntry.h"
#include "lex/Lexer.h"


namespace syntax::lr
{
    enum LrOption
    {
        shift,
        reduce,
        accept,
        go
    };

    struct LrHashEntry
    {
        LrOption op;
        size_t id;

        LrHashEntry(LrOption option, size_t id) : op(option), id(id)
        {
        }

        LrHashEntry(): op(LrOption::shift), id(0)
        {
        };
    };

    struct LrEntry
    {
        SyntaxEntry* syn;
        size_t dot;
        token::TermToken look;

        LrEntry(SyntaxEntry* entry, size_t dot, token::Token lookahead) :
            syn(entry),
            dot(dot),
            look(lookahead)
        {
        }

        LrEntry(const LrEntry& other) = default;

        explicit LrEntry(SyntaxEntry* entry) :
            syn(entry),
            dot(0),
            look(token::null)
        {
        }

        inline bool operator==(const LrEntry& other) const
        {
            return syn->id == other.syn->id && dot == other.dot &&
                look == other.look;
        }

        inline bool operator<(const LrEntry& other) const
        {
            if (syn->id != other.syn->id)
                return syn->id < other.syn->id;
            if (dot != other.dot)
                return dot < other.dot;
            return look < other.look;
        }

        inline LrEntry& operator++()
        {
            ++dot;
            return *this;
        }
    };

    using EntrySet = std::set<LrEntry>;
    using LrStateId = size_t;

    struct LrState
    {
        EntrySet entries;
        LrStateId id;

        LrState() : id(0)
        {
        }

        LrState(const EntrySet& entries) : entries(entries), id(0)
        {
        }

        inline bool operator==(const LrState& other) const
        {
            return entries == other.entries;
        }

        inline bool operator<(const LrState& other) const
        {
            return entries < other.entries;
        }
    };

    using LrSet = std::set<LrState>;
    using LrArray = std::vector<LrState>;
    using LrTable = std::unordered_map<LrStateId, std::unordered_map<size_t, LrHashEntry>>;

    LrArray& getStates();
    LrTable& getLrTable();

    void initLr();

    void saveTable();
    void loadTable();
}
