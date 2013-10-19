#include "ParserTestBase.h"

class ParserTest_Loops : public ParserTestBase {
};

TEST_F(ParserTest_Loops, InfiniteLoop) {
    Three::ASTNode* node;

    node = this->parse("def test()\nloop\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_LOOP(node);
}

TEST_F(ParserTest_Loops, SingleConditionLoop) {
    ASTNode* node;

    node = this->parse("def test(Int a)\nloop a\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    LoopNode* loop = dynamic_cast<Three::LoopNode*>(node);
    ASSERT_LOOP(loop);
    ASSERT_VARIABLE_NODE("Int",  0, "a", loop->condition());
}

TEST_F(ParserTest_Loops, LoopAfter) {
    ASTNode* node;

    node = this->parse("def test(Int a)\nloop:after a\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    LoopNode* loop = dynamic_cast<Three::LoopNode*>(node);
    ASSERT_LOOP(loop);
    ASSERT_TRUE(loop->evaluateConditionAtEnd());
    ASSERT_VARIABLE_NODE("Int",  0, "a", loop->condition());
}

TEST_F(ParserTest_Loops, ForStatement) {
    ASTNode* node;

    node = this->parse("def test()\nfor (Int i = 0; i < 10; i += 1)\nend\nend\n");
    node = node->childAtIndex(0);

    ForNode* loop = dynamic_cast<Three::ForNode*>(node->childAtIndex(0));
    ASSERT_EQ("For", loop->nodeName());

    ASSERT_VARIABLE_DECLARATION("Int", 0, "i", loop->startExpression());
    ASSERT_OPERATOR("<", loop->condition());
    ASSERT_OPERATOR("+=", loop->loopExpression());
}

TEST_F(ParserTest_Loops, ForRangeStatement) {
    ASTNode* node;

    node = this->parse("def test()\nfor (Int i in 0:10)\nend\nend\n");
    node = node->childAtIndex(0);

    Three::ForNode* loop = dynamic_cast<Three::ForNode*>(node->childAtIndex(0));
    ASSERT_EQ("For", loop->nodeName());
    ASSERT_VARIABLE_DECLARATION("Int", 0, "i", loop->startExpression());
    ASSERT_INTEGER_LITERAL_NODE(0, loop->rangeStartExpression());
    ASSERT_INTEGER_LITERAL_NODE(10, loop->rangeEndExpression());
}

TEST_F(ParserTest_Loops, BreakStatement) {
    ASTNode* node;

    node = this->parse("def test(Int a)\nloop a\nbreak\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    Three::LoopNode* loop = dynamic_cast<Three::LoopNode*>(node);
    ASSERT_LOOP(loop);

    ASSERT_EQ("Break", loop->childAtIndex(0)->nodeName());
}

TEST_F(ParserTest_Loops, ContinueStatement) {
    ASTNode* node;

    node = this->parse("def test(Int a)\nloop a\ncontinue\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    Three::LoopNode* loop = dynamic_cast<Three::LoopNode*>(node);
    ASSERT_LOOP(loop);

    ASSERT_EQ("Continue", loop->childAtIndex(0)->nodeName());
}
