#include "SyntaxLl.h"

#include <iomanip>

using namespace std;
using namespace token;
using namespace syntax;
using namespace syntax::ll;

inline FirstArray& syntax::ll::getFirstArray(){
    static FirstArray firstArray(tokenCount);
    return firstArray;
}

inline FirstArray& syntax::ll::getFollowArray(){
    static FollowArray followArray(tokenCount);
    return followArray;
}

void syntax::ll::searchNull()
{
    auto& firstArray = getFirstArray();
    // 情况1
    // A -> null
    // 遍历所有产生式，如果产生式以空串开始,
    // 则将当前符号的 first 集合标记为为允许 null
    for (auto &syntax: getSyntaxes())
        if (syntax.isNull() ) 
            firstArray[syntax.l].canNull = true;
        
    
    size_t count;
    do {
        count = 0;
        // 情况2
        // A -> BC
        // B -> null
        // C -> null
        // D -> A
        // 遍历所有产生式，如果产生式中的所有符号都可进一步推导出空串
        // 则将当前符号的 first 集合标记为为允许 null
        for (auto &syntax: getSyntaxes()) {
            bool flag = true;
            // 去掉所有不为空串的情况
            // 1. 终结符
            // 2. 非终结符不为 null
            for (auto &token: syntax.r) {
                if (token.isTerminal() || !firstArray[token].canNull) {
                    flag = false;
                    break;
                }
            }
            // 记录本次更新次数
            size_t t = syntax.l;
            if (flag && !firstArray[t].canNull) {
                count += 1;
                firstArray[t].canNull = true;
            }
        }
    } while (count);
    
}

void syntax::ll::searchFirst(){
    auto& firstArray = getFirstArray();
    
    // 所有终结符的 first 包含自身
    for (int i = nonTerminalCount; i < tokenCount; ++i) {
        if (TokenState::null == i) continue;
        firstArray[i].set.emplace(i);
    }
    
    size_t count;
    do {
        count = 0;
        // 遍历所有产生式
        for (auto &syntax: getSyntaxes()) {
            // 1. 固定要加入的符号
            for (size_t j = 0; j < syntax.r.size(); ++j) {
                // 2. 考虑之前的所有符号是否可以都为 null
                // 如果可以则将当前符号的 first 集合加入
                // A -> BCDE
                // B -> null
                // C -> null
                // D -> a

                // 第一个一定可以加入
                if(0 == j)
                {
                    count += mergeFirst(syntax, j);
                    continue;
                }

                // 连续判断，只需判断前一个
                Token before = syntax.r[j - 1];
                // 非终结符且可以为 null
                if (!before.isTerminal() && firstArray[before].canNull) 
                    count += mergeFirst(syntax, j);
                // 如果不可以，之后也一定不可以，跳过
                else break;
            }
        }
    } while (count);
}

void syntax::ll::searchFollow() {
    auto& firstArray = getFirstArray();
    auto& followArray = getFollowArray();
    
    // 将起始符号的Follow集合设置为 $
    followArray[0].set.emplace(TokenState::real_end);
    
    size_t count;
    do{
        count = 0;
        // 遍历所有产生式
        for (auto &syntax: getSyntaxes()) {
            // 遍历所有符号
            // A -> BCD
            // C -> null
            for (size_t j = 0; j < syntax.r.size(); ++j) {
                Token l = syntax.r[j];
                if (l.isTerminal()) continue;
                
                // 更新所有局部符号的Follow集合
                // 如果中间符号为空串，则将当前符号的 first 集合加入到目标符号的 follow 集合中
                for (size_t k = j + 1; k < syntax.r.size(); ++k) {
                    Token r = syntax.r[k];

                    if (j+1 == k)
                    {
                        auto& from = firstArray[r];
                        auto& to = followArray[l];
                        count += to.merge(from);
                        continue;
                    }
                    
                    // 检查上一个符号
                    Token pre = syntax.r[k - 1];
                    if(pre.isTerminal() || !firstArray[pre].canNull)
                        break;

                    auto& from = firstArray[r];
                    auto& to = followArray[l];
                    count += to.merge(from);
                    
                }
            }

            // 检查所有后续符号为空串的情况, 从后向前遍历
            // 此时将当前符号的 follow 集合加入到 l 的 follow 集合中
            for (auto it = syntax.r.rbegin(); it != syntax.r.rend(); ++it) {
                if (it->isTerminal() || !firstArray[*it].canNull) {
                    break;
                }
                auto& from = firstArray[syntax.l];
                auto& to = followArray[*it];
                count += to.merge(from);
            }

        }
    } while (count);
}

size_t syntax::ll::mergeFirst(const SyntaxEntry& syntax, size_t loc) {
    // TODO: add syntax index
    auto& to = getFirstArray()[syntax.l];
    auto& from = getFirstArray()[syntax.r[loc]];
    return to.merge(from);
}

size_t syntax::ll::FirstEntry::merge(const FirstEntry& other)
{
    size_t pre = set.size();
    set.insert(other.set.begin(), other.set.end());
    size_t post = set.size();
    return (post - pre);
}


void syntax::ll::initFirst()
{
    searchNull();
    searchFirst();
    searchFollow();
}

static void printLine() {
    string separator(40, '_');
    cout << separator << '\n';
}

void syntax::ll::printFirst()
{
    cout << "First Table:" << '\n';
    printLine();
    for (size_t i = 0; i < nonTerminalCount; ++i)
    {
        auto& entry = getFirstArray()[i];
        if (entry.canNull) cout << "* ";
        else cout << "  ";
        cout << left << setw(15) << NonTermToken(i)  << " -> ";
        for( auto& t: entry.set)
        {
            cout << t.token << " ";
        }
        cout << '\n';
    }
    cout << '\n';
}

void syntax::ll::printFollow()
{
    cout << "Follow Table:" << '\n';
    printLine();
    for (size_t i = 0; i < nonTerminalCount; ++i)
    {
        cout << left << setw(15) << NonTermToken(i)  << " -> ";
        for( auto& t: getFollowArray()[i].set)
        {
            cout << t.token << " ";
        }
        cout << '\n';
    }
    cout << '\n';
}