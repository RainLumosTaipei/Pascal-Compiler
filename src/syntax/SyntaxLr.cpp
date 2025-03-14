#include <chrono>
#include <iomanip>

#include "syntax/SyntaxLr.h"
#include "syntax/SyntaxLl.h"
#include "syntax/Serialize.h"


using namespace std;
using namespace token;
using namespace syntax::lr;
using namespace syntax::ll;
using namespace syntax;


inline LrArray& syntax::lr::getStates()
{
    static LrArray states;
    return states;
}

inline LrTable& syntax::lr::getLrTable()
{
    static LrTable lrTable;
    return lrTable;
}

static LrSet statesSet;

static inline void printTime()
{
    auto now = std::chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&currentTime);
    cout << put_time(localTime, "[%H:%M:%S]");
}

static void searchEntry(EntrySet& entries)
{
    size_t count;
    EntrySet newEntries;
    EntrySet extEntries = entries;
    do
    {
        count = 0;
        newEntries.clear();

        // 遍历所有生成式
        for (auto& entry : extEntries)
        {
            // 找到还可以移动的生成式
            // A -> .BCd
            SyntaxEntry* psyntax = entry.syn;
            if (entry.dot >= psyntax->r.size())
            {
                continue;
            }
            // 下一个符号为非终结符才可以移动
            Token nextToken = psyntax->r[entry.dot];
            if (!nextToken.isTerminal())
            {
                // 获取lookahead的终结符
                set<TermToken> looks;

                // 将当前 lookahead 加入到 set 中
                if (TokenState::null != entry.look)
                    looks.insert(entry.look);

                // 如果当前符号不是最后一个符号，则将当前符号的First集合加入到lookahead中
                // A -> .BCDe  添加 first(CD)
                for (size_t loc = entry.dot; loc < psyntax->r.size() - 1; ++loc)
                {
                    Token ahead = psyntax->r[loc + 1];
                    auto& aheadFirst = getFirstArray()[ahead];
                    for (auto& firstToken : aheadFirst.set)
                    {
                        looks.insert(firstToken.token);
                    }
                    // 如果是终结符或者不为null，则直接跳出循环
                    if (ahead.isTerminal() || !aheadFirst.canNull) break;
                }

                // 遍历所有产生式，寻找可以推导的
                for (auto& syntax : getSyntaxes())
                {
                    if (syntax.l == nextToken)
                    {
                        // 遍历 lookahead
                        for (auto& look : looks)
                        {
                            newEntries.emplace(&syntax, 0, look);
                        }
                    }
                }
            }
        }

        if (!newEntries.empty())
        {
            size_t pre = entries.size();
            entries.insert(newEntries.begin(), newEntries.end());
            size_t post = entries.size();
            count = post - pre;
        }
        extEntries = newEntries;
    }
    while (count);
}

static void searchStates()
{
    auto& states = statesSet;

    {
        // 添加初始节点
        auto firstState = new LrState();
        // 为初始节点添加第一个生成式
        firstState->entries.emplace(&getSyntaxes()[0]);
        // 生成第一个 lr 状态
        searchEntry(firstState->entries);
        states.insert(*firstState);
        delete firstState;
        cout << "\nStart building LR table......\n\n";
    }

    size_t count;
    size_t times = 0;
    LrSet extStates = statesSet;
    LrSet newStates;
    do
    {
        count = 0;

        newStates.clear();
        // 遍历所有状态
        for (auto& state : extStates)
        {
            // 选定下一个 token
            for (int nextToken = 0; nextToken < tokenCount; ++nextToken)
            {
                if (nextToken == TokenState::null || nextToken == TokenState::real_end)
                {
                    continue;
                }

                // 遍历当前 state 所有生成式，添加所有符合的表达式
                EntrySet newEntry;
                for (auto& entry : state.entries)
                {
                    if (entry.dot >= entry.syn->r.size()) continue;

                    // 下一个token正确, 拷贝加入新生成式
                    if (nextToken == (int)entry.syn->r[entry.dot])
                    {
                        auto e(entry);
                        newEntry.insert(++e);
                    }
                }

                if (!newEntry.empty())
                {
                    // 填充新状态
                    searchEntry(newEntry);
                    newStates.emplace(newEntry);
                }
            }
        }

        if (!newStates.empty())
        {
            size_t pre = states.size();
            states.insert(newStates.begin(), newStates.end());
            size_t post = states.size();
            count = post - pre;

            {
                printTime();
                cout << " No." << left << setw(2) << ++times
                    << " new " << left << setw(20) << string(count / 50 + 1, '=')
                    << left << setw(3) << count
                    << "  |  total " << left << setw(20) << string(post / 500 + 1, '=') << post << '\n';
            }
        }
        extStates = newStates;
    }
    while (count);
}

static size_t findState(EntrySet& entries)
{
    for (auto& state : getStates())
    {
        if (entries == state.entries)
            return state.id;
    }
    return 0;
}

static void fillLrTable()
{
    auto& table = getLrTable();
    auto& states = getStates();

    table.reserve(statesSet.size());
    states.resize(statesSet.size());
    copy(statesSet.begin(), statesSet.end(), states.begin());
    size_t index = 0;
    for (auto& state : states) state.id = index++;


    // 遍历所有状态
    for (auto& state : states)
    {
        // 选定下一个 token
        for (int nextToken = 0; nextToken < tokenCount; ++nextToken)
        {
            if (nextToken == TokenState::null || nextToken == TokenState::real_end)
            {
                continue;
            }
            // 遍历当前 state 所有生成式
            EntrySet newEntry;
            for (auto& entry : state.entries)
            {
                auto* pentry = entry.syn;

                // reduce
                // 到达生成式末尾，可以规约
                if (entry.dot >= pentry->r.size())
                {
                    // 只执行一次
                    if (0 != nextToken) continue;

                    // null 不应在可以从 first 推导的产生式中规约
                    if (entry.syn->r.empty())
                    {
                        auto& first = getFirstArray()[entry.syn->l];
                        if (first.find(entry.look)) continue;
                    }

                    // TODO: 移进规约冲突可能需要手动解决
                    // 存在 shift 冲突则插入失败
                    table[state.id].emplace(entry.look, LrHashEntry(LrOption::reduce, pentry->id));
                    continue;
                }

                // 下一个token正确, 拷贝加入新生成式
                if (nextToken == (int)entry.syn->r[entry.dot])
                {
                    auto e(entry);
                    newEntry.insert(++e);
                }
            }
            if (!newEntry.empty())
            {
                searchEntry(newEntry);
                size_t id = findState(newEntry);
                // shift && goto
                if (Token(nextToken).isTerminal())
                    table[state.id][nextToken] = LrHashEntry(LrOption::shift, id);
                else
                    table[state.id][nextToken] = LrHashEntry(LrOption::go, id);
            }
        }
        // accept
        if (state.id != 0 && state.entries.size() == 1)
        {
            for (auto& entry : state.entries)
                if (TokenState::null == entry.look)
                    table[state.id][TokenState::real_end] = LrHashEntry(LrOption::accept, 0);
        }
    }

    cout << "\nLR table is ready!\n\n";
}

void syntax::lr::saveTable()
{
    const auto& table = getLrTable();
    serializeLrTable(table);
}

void syntax::lr::loadTable()
{
    auto& table = getLrTable();
    deserializeLrTable(table);
}

void syntax::lr::initLr()
{
    searchStates();
    fillLrTable();
}
