#pragma once

#include "SyntaxLr.h"

namespace syntax::lr
{
    void serializeLrTable(const LrTable& table);
    int deserializeLrTable(LrTable& table);
}
