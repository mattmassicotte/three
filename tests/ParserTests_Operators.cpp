#include "ParserTestBase.h"

class ParserTest_Operators : public ParserTestBase {
};

TEST_F(ParserTest_Operators, DereferenceUnaryOperator) {
    Language::ASTNode* node;

    node = this->parse("def test(*Int a)\n*a = null\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_OPERATOR("*", node->childAtIndex(0));
    ASSERT_EQ(1, node->childAtIndex(0)->childCount());
}

TEST_F(ParserTest_Operators, AddressOfUnaryOperator) {
    Language::ASTNode* node;

    node = this->parse("def test(*Int a)\na = &a\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_OPERATOR("&", node->childAtIndex(1));
    ASSERT_EQ(1, node->childAtIndex(1)->childCount());
}