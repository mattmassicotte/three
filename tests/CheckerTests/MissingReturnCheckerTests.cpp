#include "../ParserTestBase.h"
#include "compiler/Operations/MissingReturnChecker.h"


class MissingReturnCheckerTests : public ParserTestBase {
};

TEST_F(MissingReturnCheckerTests, FunctionWithEnsure) {
    ASTNode* node = this->parse("def test()\n"
                                "  return\n"
                                "ensure\n"
                                "  Int x = 5\n"
                                "end\n");

    MissingReturnChecker checker;

    node->accept(checker);
}

TEST_F(MissingReturnCheckerTests, FunctionWithoutReturnType) {
    ASTNode* node = this->parse("def test()\n"
                                "end\n");

    MissingReturnChecker checker;

    node->accept(checker);
}

TEST_F(MissingReturnCheckerTests, FunctionWithReturnType) {
    ASTNode* node = this->parse("def test(; Int)\n"
                                "  return 5\n"
                                "end\n");

    MissingReturnChecker checker;

    node->accept(checker);
}

TEST_F(MissingReturnCheckerTests, FunctionMissingReturn) {
    ASTNode* node = this->parse("def test(; Int)\n"
                                "end\n");

    MissingReturnChecker checker;

    node->accept(checker);
}
