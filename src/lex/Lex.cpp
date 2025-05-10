#include <fstream>
#include <sstream>
#include <filesystem>

#include "lex/Lexer.h"
#include "lex/LexErr.h"
#include "semantic/Ast.h"

using namespace std;
using namespace token;

inline TokenDeque& token::getTokens()
{
    static TokenDeque t;
    return t;
}

int token::lex(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Failed to open file: " << filename << '\n';
        cerr << "Current working directory: " << filesystem::current_path() << '\n';
        cerr << "Please make sure the file exists in that directory." << '\n';
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string test = buffer.str();

    Lexer lexer(test);
    Lexer::filename = filename;
    ast::getModule().setSourceFileName(filename);
    auto& tokens = getTokens();

    try
    {
        TokenDesc* s;
        do
        {
            s = lexer.getNextToken();
            tokens.push_back(s);
        }
        while (s->token != real_end);
    }
    catch (LexErr& err)
    {
        cout << err << endl;
        return 1;
    }

    return 0;
}


void token::printTokens()
{
    auto& tokens = getTokens();
    while (!tokens.empty())
    {
        auto& t = tokens.front()->token;
        cout << t << " ";
        if (p_semicolon == t || key_begin == t)
            cout << endl;
        tokens.pop_front();
    }
    cout << endl << endl;
}
