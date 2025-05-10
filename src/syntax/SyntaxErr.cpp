#include "syntax/SyntaxErr.h"

using namespace std;
using namespace syntax;
using namespace token;

SyntaxErr::SyntaxErr(std::string msg, const TokenDesc* desc)
    : msg(std::move(msg)), line(desc->line), col(desc->col), name(desc->str)
{
}
