#include <fstream>
#include <sstream>
#include <filesystem>

#include "lex/Lexer.h"

using namespace std;
using namespace token;

inline TokenDeque& token::getTokens(){
    static TokenDeque t;
    return t;
}

void token::lex()
{
    string test = "program main; "
                  "  var "
                  "      a: integer; "
                  "      b, c: real; "
                  "begin "
                  "  a := 3;"
                  "  c := 6; "
                  "end.";

    Lexer lexer(test);
    auto& tokens = getTokens();

    TokenDesc* s;
    do {
        s = lexer.getNextToken();
        tokens.push_back(s);
    } while (s->token != TokenState::real_end);
}


void token::lex(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << '\n';
        cerr << "Current working directory: " << filesystem::current_path() << '\n';
        cerr << "Please make sure the file exists in that directory." << '\n';
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string test = buffer.str();

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
        auto& t = tokens.front()->token;
        cout << t << " ";
        if(TokenState::p_semicolon == t || TokenState::key_begin == t)
            cout << '\n';
        tokens.pop_front();
    }
    cout << "\n\n" ;
}