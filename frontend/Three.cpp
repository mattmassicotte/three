#include "../compiler/Parser.h"

#include <iostream>
#include <fstream>

Language::RootNode* createRootNodeFromFile(const std::string& path) {
    std::ifstream inputFile(path);

    Language::Lexer lexer(&inputFile);
    Language::Parser parser(&lexer);

    return parser.rootASTNode();
}

bool createCSource(Language::RootNode* node) {
    Language::CSourceContext c;

    node->codeGenCSource(c);

    std::cout << c.renderToString();

    return true;
}

int main(int argc, char** argv) {
    Language::RootNode* node;

    node = createRootNodeFromFile(std::string(argv[1]));

    createCSource(node);

    return 0;
}
