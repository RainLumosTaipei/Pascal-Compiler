#include "lex/Lexer.h"
#include "syntax/SyntaxLl.h"
#include "syntax/SyntaxLr.h"
#include "syntax/SyntaxCheck.h"
#include "syntax/ast.h"
#include "codegen/CodeGenerator.h"  // 新增头文件
#include <fstream>                  // 新增文件流支持
#include <iostream>

using namespace std;

static void init()
{
    syntax::ll::initFirst();
    syntax::lr::initLr();
    syntax::lr::saveTable();
    syntax::ll::printFirst();
    syntax::ll::printFollow();
}

static void test(const string& fileName)
{
    syntax::lr::loadTable();
    token::lex(fileName);
    //token::printTokens();
    syntax::lr::lrCheck();
}

void printAst(const syntax::Ast* node, int depth = 0) {
    if (!node) return;
    // 缩进表示层次
    for (int i = 0; i < depth; ++i) cout << "  ";
    // 打印节点信息
    if (node->t) {
        cout << "TokenType: " << *(node->t)
             << ", Value: '" << node->t->str
             << "', Line: " << node->t->line
             << ", Col: " << node->t->col << endl;
    } else {
        cout << "Token: (null)" << endl;
    }
    // 递归子节点
    for (const auto& child : node->child) {
        printAst(child, depth + 1);
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    syntax::initSyntaxes();
    test(argv[1]);

    // 获取AST根节点
    syntax::Ast* root = syntax::getAstHead();
    if (!root) {
        cerr << "Error: AST is empty!" << endl;
        return 1;
    }

    // 打印AST
    cout << "Abstract Syntax Tree (AST):" << endl;
    printAst(root);

    // 生成C代码
    // main.cpp 修改后的生成代码部分
    try {
        pascal2c::codegen::CodeGenerator generator;
        string cCode = generator.generate(root);

        // 打印到控制台
        cout << "\n=== 生成的C代码 ===\n" << cCode << "\n===================\n";

        // 输出到文件
        string outputFile = "output.c";
        ofstream out(outputFile);
        if (out.is_open()) {
            out << cCode;
            cout << "Generated C code saved to: " << outputFile << endl;
        } else {
            cerr << "无法打开输出文件！" << endl;
        }

    } catch (const exception& e) {
        cerr << "代码生成失败: " << e.what() << endl;
        return 1;
    }

    return 0;
}