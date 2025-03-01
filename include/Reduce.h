#pragma once

#include <unordered_map>

namespace syntax{

    typedef void (*ReduceFn)();

    typedef std::unordered_map<int, ReduceFn> ReduceTable;
}
