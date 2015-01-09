#include "../ParserTestsBase.h"

class ParserTests_Variables : public ParserTestsBase {
};

TEST_F(ParserTests_Variables, AssignToGlobal) {
    Three::ASTNode* node = this->parseNodeWithBodies("Int global\n"
                                                     "def test()\n"
                                                     "  global = 5\n"
                                                     "end\n");

    ASSERT_EQ(2, node->childCount());
    auto variableDec = dynamic_cast<VariableDeclarationNode*>(node->childAtIndex(0));
    ASSERT_EQ("Variable Declaration", variableDec->nodeName());
    ASSERT_TRUE(variableDec->global);

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("Global Variable", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Variables, AssignToNamespacedGlobal) {
    Three::ASTNode* node = this->parseNodeWithBodies("namespace Foo\n"
                                                     "  Int global\n"
                                                     "end\n"
                                                     "def test()\n"
                                                     "  Foo::global = 5\n"
                                                     "end\n");

    ASSERT_EQ(2, node->childCount());
    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("Global Variable", node->childAtIndex(0)->nodeName());
}
