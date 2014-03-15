#include "../ParserTestBase.h"
#include "compiler/Operations/EnsureReturnChecker.h"


class EnsureReturnCheckerTests : public ParserTestBase {
};

TEST_F(EnsureReturnCheckerTests, ValidEnsure) {
    ASTNode* node = this->parse("def test()\n"
                                "  return\n"
                                "ensure\n"
                                "  Int x = 5\n"
                                "end\n");

    EnsureReturnChecker checker;

    node->accept(checker);
}

TEST_F(EnsureReturnCheckerTests, InvalidEnsure) {
    ASTNode* node = this->parse("def test()\n"
                                "  return\n"
                                "ensure\n"
                                "  return\n"
                                "end\n");

    EnsureReturnChecker checker;

    node->accept(checker);
}

TEST_F(EnsureReturnCheckerTests, ReturnInClosure) {
    ASTNode* node = this->parse("def test()\n"
                                "  return\n"
                                "ensure\n"
                                "  return\n"
                                "end\n");

    EnsureReturnChecker checker;

    node->accept(checker);
}
