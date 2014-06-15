#include "../ParserTestsBase.h"

class ParserTests_Loops : public ParserTestsBase {
};

TEST_F(ParserTests_Loops, InfiniteLoop) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  loop\n"
                                              "  end\n"
                                              "end\n");

    LoopNode* loop = dynamic_cast<LoopNode*>(node->childAtIndex(0));

    ASSERT_EQ("Loop", loop->nodeName());
    ASSERT_EQ(0, loop->childCount());
    ASSERT_EQ(nullptr, loop->condition());
    ASSERT_FALSE(loop->evaluateConditionAtEnd());
}

TEST_F(ParserTests_Loops, SingleConditionLoop) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  loop a\n"
                                              "  end\n"
                                              "end\n");

    LoopNode* loop = dynamic_cast<LoopNode*>(node->childAtIndex(0));

    ASSERT_EQ("Loop", loop->nodeName());
    ASSERT_EQ(0, loop->childCount());
    ASSERT_EQ("Local Variable", loop->condition()->nodeName());
    ASSERT_FALSE(loop->evaluateConditionAtEnd());
}

TEST_F(ParserTests_Loops, LoopAfter) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  loop:after a\n"
                                              "  end\n"
                                              "end\n");

    LoopNode* loop = dynamic_cast<LoopNode*>(node->childAtIndex(0));

    ASSERT_EQ("Loop", loop->nodeName());
    ASSERT_EQ(0, loop->childCount());
    ASSERT_EQ("Local Variable", loop->condition()->nodeName());
    ASSERT_TRUE(loop->evaluateConditionAtEnd());
}

TEST_F(ParserTests_Loops, ForStatement) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  for (Int i = 0; i < 10; i += 1)\n"
                                              "  end\n"
                                              "end\n");

    ForNode* loop = dynamic_cast<ForNode*>(node->childAtIndex(0));

    ASSERT_EQ("For", loop->nodeName());
    ASSERT_EQ("Variable Declaration", loop->startExpression()->nodeName());
    ASSERT_EQ("Less-Than Operator", loop->condition()->nodeName());
    ASSERT_EQ("Add-Assign Operator", loop->loopExpression()->nodeName());
}

TEST_F(ParserTests_Loops, ForRangeStatement) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  for (Int i in 0:10)\n"
                                              "  end\n"
                                              "end\n");

    ForNode* loop = dynamic_cast<ForNode*>(node->childAtIndex(0));

    ASSERT_EQ("For", loop->nodeName());
    ASSERT_EQ("Variable Declaration", loop->startExpression()->nodeName());
    ASSERT_EQ(nullptr, loop->condition());
    ASSERT_EQ(nullptr, loop->loopExpression());

    ASSERT_EQ("Integer Literal", loop->rangeStartExpression()->nodeName());
    ASSERT_EQ(0, dynamic_cast<IntegerLiteralNode*>(loop->rangeStartExpression())->value());
    ASSERT_EQ("Integer Literal", loop->rangeEndExpression()->nodeName());
    ASSERT_EQ(10, dynamic_cast<IntegerLiteralNode*>(loop->rangeEndExpression())->value());
}

TEST_F(ParserTests_Loops, BreakStatement) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  loop\n"
                                              "    break\n"
                                              "  end\n"
                                              "end\n");

    LoopNode* loop = dynamic_cast<LoopNode*>(node->childAtIndex(0));

    ASSERT_EQ("Loop", loop->nodeName());
    ASSERT_EQ(nullptr, loop->condition());

    ASSERT_EQ("Break", loop->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Loops, BreakStatementTailingIf) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  loop\n"
                                              "    break if a\n"
                                              "  end\n"
                                              "end\n");

    LoopNode* loop = dynamic_cast<LoopNode*>(node->childAtIndex(0));

    ASSERT_EQ("Loop", loop->nodeName());
    ASSERT_EQ(nullptr, loop->condition());

    node = loop->childAtIndex(0);
    ASSERT_EQ("If", node->nodeName());
    ASSERT_EQ("Break", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Loops, ContinueStatement) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  loop\n"
                                              "    continue\n"
                                              "  end\n"
                                              "end\n");

    LoopNode* loop = dynamic_cast<LoopNode*>(node->childAtIndex(0));

    ASSERT_EQ("Loop", loop->nodeName());
    ASSERT_EQ(nullptr, loop->condition());

    ASSERT_EQ("Continue", loop->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Loops, ContinueStatementTailingIf) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  loop\n"
                                              "    continue if a\n"
                                              "  end\n"
                                              "end\n");

    LoopNode* loop = dynamic_cast<LoopNode*>(node->childAtIndex(0));

    ASSERT_EQ("Loop", loop->nodeName());
    ASSERT_EQ(nullptr, loop->condition());

    node = loop->childAtIndex(0);
    ASSERT_EQ("If", node->nodeName());
    ASSERT_EQ("Continue", node->childAtIndex(0)->nodeName());
}
