#include "semantic/Reduce.h"
#include <stack>
#include "syntax/SyntaxCheck.h"


using namespace std;
using namespace semantic;
using namespace token;


inline ReduceTable& getReduceTable()
{
    static ReduceTable reduceTable;
    return reduceTable;
}

TokenDesc* semantic::callReduce(size_t id, TokenState t)
{
    auto& table = getReduceTable();
    auto desc = new TokenDesc(t);
    if (table.find(id) == table.end()) return desc;
    table[id](desc);
    return desc;
}
