#include "syntax/SyntaxCheck.h"
#include "syntax/SyntaxLr.h"
#include "semantic/Reduce.h"
#include "cmd.h"

#include <stack>
#include <iomanip>

using namespace std;
using namespace syntax;
using namespace syntax::lr;
using namespace token;

static stack<LrStateId> stateStack;

inline deque<TokenDesc*>& syntax::lr::getWaitTokens()
{
    static deque<TokenDesc*> waitTokens;
    return waitTokens;
}

namespace
{
    void printDeque(deque<TokenDesc*>& deq, int dir)
    {
        size_t times = 0;
        if (1 == dir)
        {
            for (auto& it : deq)
            {
                cout << it->token << " ";
                if (++times >= 10) break;
            }
            return;
        }
        vector<TokenDesc*> waitArray;
        for (auto it = deq.rbegin(); it != deq.rend(); ++it)
        {
            waitArray.push_back(*it);
            if (++times >= 10) break;
        }
        for (auto it = waitArray.rbegin(); it != waitArray.rend(); ++it)
        {
            cout << (*it)->token << " ";
        }
    }

    void check()
    {
        printDeque(getWaitTokens(), 0);
        cout << "  <=====>  ";
        printDeque(getTokens(), 1);
        cout << endl << endl;
    }

    void printError()
    {
        auto& tokens = getTokens();
        auto& t = tokens.front();
        cout << "Error: "
            << "possible token is wrong " + t->str
            << " at line " << t->line
            << ", col " << t->col
            << '\n';
    }
}

int syntax::lr::lrCheck()
{
    auto& table = getLrTable();
    auto& tokens = getTokens();
    int isAccept = 1;
    size_t times = 0;
    
    // 初始 lr 状态
    stateStack.push(0);
    // 开始进行分析
    while (!stateStack.empty())
    {
        if(debugFlag)
            check();

        int curToken = tokens.front()->token;
        LrStateId curState = stateStack.top();

        // 查找 lr 分析表
        LrHashEntry option;
        if (table.find(curState) != table.end() &&
            table[curState].find(curToken) != table[curState].end())
        {
            option = table[curState][curToken];
        }
        else goto end;


        // 判断操作
        switch (option.op)
        {
        // goto
        case go:
            goto end;

        // accept
        case accept:
            if(debugFlag)
                cout << "No." << left << setw(3) << ++times << " accept (*^__^*) " << '\n';
            isAccept = 0;
            goto end;

        // shift
        case shift:
            if(debugFlag)
                cout << "No." << left << setw(3) << ++times << " shift" << '\n';

            // 移进 token
            getWaitTokens().push_back(tokens.front());
            tokens.pop_front();
            // 压入新的状态
            stateStack.push(option.id);
            break;

        // reduce
        case reduce:
            auto& entry = getSyntaxes()[option.id];

            if(debugFlag)
                cout << "No." << left << setw(3) << ++times << " reduce " << option.id << " ["
                << entry << "]\n";

            // 规约操作
            TokenDesc* newT = semantic::callReduce(option.id, entry.l);


            // 弹出符号和状态
            for (size_t i = 0; i < entry.r.size(); ++i)
            {
                getWaitTokens().pop_back();
                stateStack.pop();
            }

            getWaitTokens().push_back(newT);

            // goto 跳转状态
            auto s = stateStack.top();
            if (table.find(s) != table.end() &&
                table[s].find(entry.l) != table[s].end())
            {
                stateStack.push(table[s][entry.l].id);
            }
            else goto end;
            break;
        }
    }

end:
    if (isAccept) printError();
    
    tokens.clear();
    while (!getWaitTokens().empty()) getWaitTokens().pop_back();
    while (!stateStack.empty()) stateStack.pop();

    return isAccept;
}
