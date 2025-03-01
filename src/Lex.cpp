#include "Lexer.h"

using namespace std;
using namespace token;

inline TokenDeque& token::getTokens(){
    static TokenDeque t;
    return t;
}

string test = "program main; "
              "  const app = 100; "
              "  var "
              "      a: integer; "
              "      b, c: real; "
              "begin "
              "  a := 3;"
              "  c := 6; "
              "end.";

//string test = "program main; "
//              "  var "
//              "      a: integer; "
//              "      b, c: real; "
//              "begin "
//              "  a := ( 3 + 6 ) * 9 + 10;"
//              "  c := ( a > b ) mod 99; "
//              "end.";

void token::lex()
{
    Lexer lexer(test);
    auto& tokens = getTokens();

    TokenDesc* s;
    do {
        s = lexer.getNextToken();
        tokens.push_back(s);
    } while (s->token != TokenState::real_end);
}

void token::printTokens()
{
    auto& tokens = getTokens();
    while (!tokens.empty())
    {
        cout << tokens.front()->token << " ";
        tokens.pop_front();
    }
    cout << "\n\n" ;
}