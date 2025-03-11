#pragma once

#include <unordered_map>

namespace semantic{

    typedef void (*ReduceFn)();
    

    typedef std::unordered_map<size_t, ReduceFn> ReduceTable;

    void callReduce(size_t id);
}
