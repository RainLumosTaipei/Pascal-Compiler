#include "semantic/SemanticErr.h"

#include "lex/Lexer.h"

using namespace std;
using namespace semantic;
using namespace token;

SemanticErr::SemanticErr(std::string msg, const token::TokenDesc* desc)
    : msg(std::move(msg)), line(desc->line), col(desc->col), name(desc->str)
{
}
