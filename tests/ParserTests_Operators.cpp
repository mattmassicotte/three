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

TEST_F(ParserTest_Operators, MemberAccessOperator) {
    Language::ASTNode* node;

    node = this->parse("struct MyStruct\nInt a\nend\ndef test(MyStruct value)\n value.a = 0\nend\n");
    node = node->childAtIndex(1)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_INTEGER_LITERAL_NODE(0, node->childAtIndex(1));

    Three::MemberAccessNode* memberAccess = dynamic_cast<Three::MemberAccessNode*>(node->childAtIndex(0));
    ASSERT_EQ("MemberAccess", memberAccess->nodeName());
    ASSERT_EQ("a", memberAccess->memberName());
    ASSERT_TRUE(!memberAccess->indirect());
    ASSERT_VARIABLE_NODE("MyStruct",  0, "value", memberAccess->childAtIndex(0));
}

TEST_F(ParserTest_Operators, IndirectMemberAccessOperator) {
    Language::ASTNode* node;

    node = this->parse("struct MyStruct\nInt a\nend\ndef test(*MyStruct value)\n value->a = 0\nend\n");
    node = node->childAtIndex(1)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_INTEGER_LITERAL_NODE(0, node->childAtIndex(1));

    Three::MemberAccessNode* memberAccess = dynamic_cast<Three::MemberAccessNode*>(node->childAtIndex(0));
    ASSERT_EQ("MemberAccess", memberAccess->nodeName());
    ASSERT_EQ("a", memberAccess->memberName());
    ASSERT_TRUE(memberAccess->indirect());
    ASSERT_VARIABLE_NODE("MyStruct",  1, "value", memberAccess->childAtIndex(0));
}
