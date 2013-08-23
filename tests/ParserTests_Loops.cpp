#include "ParserTestBase.h"

class ParserTest_Loops : public ParserTestBase {
};

TEST_F(ParserTest_Loops, InfiniteLoop) {
    Language::ASTNode* node;

    node = this->parse("def test()\nloop\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_LOOP(node);
}

TEST_F(ParserTest_Loops, SingleConditionLoop) {
    Language::ASTNode* node;

    node = this->parse("def test(Int a)\nloop a\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    Language::LoopNode* loop = dynamic_cast<Language::LoopNode*>(node);
    ASSERT_LOOP(loop);
    ASSERT_VARIABLE_NODE("Int",  0, "a", loop->condition());
}

TEST_F(ParserTest_Loops, LoopAfter) {
    Language::ASTNode* node;

    node = this->parse("def test(Int a)\nloop:after a\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    Language::LoopNode* loop = dynamic_cast<Language::LoopNode*>(node);
    ASSERT_LOOP(loop);
    ASSERT_TRUE(loop->evaluateConditionAtEnd());
    ASSERT_VARIABLE_NODE("Int",  0, "a", loop->condition());
}

TEST_F(ParserTest_Loops, ForStatement) {
    Language::ASTNode* node;

    node = this->parse("def test()\nfor (Int i = 0; i < 10; i += 1)\nend\nend\n");
    node = node->childAtIndex(0);

    Three::ForNode* loop = dynamic_cast<Three::ForNode*>(node->childAtIndex(0));
    ASSERT_EQ("For", loop->nodeName());

    ASSERT_VARIABLE_DECLERATION("Int", 0, "i", loop->startExpression());
    ASSERT_OPERATOR("<", loop->condition());
    ASSERT_OPERATOR("+=", loop->loopExpression());
}

TEST_F(ParserTest_Loops, BreakStatement) {
    Language::ASTNode* node;

    node = this->parse("def test(Int a)\nloop a\nbreak\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    Language::LoopNode* loop = dynamic_cast<Language::LoopNode*>(node);
    ASSERT_LOOP(loop);

    ASSERT_EQ("Break", loop->childAtIndex(0)->nodeName());
}

TEST_F(ParserTest_Loops, ContinueStatement) {
    Language::ASTNode* node;

    node = this->parse("def test(Int a)\nloop a\ncontinue\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    Language::LoopNode* loop = dynamic_cast<Language::LoopNode*>(node);
    ASSERT_LOOP(loop);

    ASSERT_EQ("Continue", loop->childAtIndex(0)->nodeName());
}
