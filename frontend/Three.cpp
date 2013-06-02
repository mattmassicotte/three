#include "../compiler/Parser.h"

#include <iostream>
#include <fstream>

Language::ASTNode* createRootNodeFromFile(const std::string& path) {
    std::ifstream inputFile(path);

    Language::Lexer lexer(&inputFile);
    Language::Parser parser(&lexer);

    return parser.rootASTNode();
}

int main(int argc, char** argv) {
    Language::ASTNode* node;

    node = createRootNodeFromFile(std::string(argv[1]));

    Language::CSourceContext c;

    node->codeGenCSource(c);

    std::cout << c.renderToString();

    return 0;
}
