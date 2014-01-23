#include "../ParserTestBase.h"

class ParserTest_Control : public ParserTestBase {
};

TEST_F(ParserTest_Control, SimpleIfWithNoElseStatement) {
    Three::ASTNode* node;

    node = this->parse("def test()\nif true\nreturn\nend\nend\n");

    Three::IfNode* ifNode = dynamic_cast<Three::IfNode*>(node->childAtIndex(0)->childAtIndex(0));

    ASSERT_EQ("If", ifNode->name());
    ASSERT_BOOLEAN_LITERAL_NODE(true, ifNode->condition());
    ASSERT_EQ(1, ifNode->childCount());
    ASSERT_RETURN_NODE(ifNode->childAtIndex(0));
    ASSERT_EQ(NULL, ifNode->elseStatement());
}

TEST_F(ParserTest_Control, SimpleIfWithElseStatement) {
    Three::ASTNode* node;

    node = this->parse("def test()\nif false\nreturn\nelse\nreturn\nend\nend\n");

    Three::IfNode* ifNode = dynamic_cast<Three::IfNode*>(node->childAtIndex(0)->childAtIndex(0));

    ASSERT_EQ("If", ifNode->name());
    ASSERT_BOOLEAN_LITERAL_NODE(false, ifNode->condition());
    ASSERT_EQ(1, ifNode->childCount());
    ASSERT_RETURN_NODE(ifNode->childAtIndex(0));
    ASSERT_RETURN_NODE(ifNode->elseStatement()->childAtIndex(0));
}

TEST_F(ParserTest_Control, TailingIfStatement) {
    Three::ASTNode* node;

    node = this->parse("def test()\nreturn if false\nend\n");

    Three::IfNode* ifNode = dynamic_cast<Three::IfNode*>(node->childAtIndex(0)->childAtIndex(0));

    ASSERT_EQ("If", ifNode->name());
    ASSERT_BOOLEAN_LITERAL_NODE(false, ifNode->condition());
    ASSERT_EQ(1, ifNode->childCount());
    ASSERT_RETURN_NODE(ifNode->childAtIndex(0));
}
