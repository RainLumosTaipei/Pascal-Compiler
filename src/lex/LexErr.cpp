#include "lex/LexErr.h"

using namespace std;
using namespace token;

LexErr::LexErr(std::string msg, const std::string& name)
    : msg(std::move(msg)), line(Lexer::line), col(Lexer::col), name(name)
{
}
