#include "../NewParserTestsBase.h"

class NewParserTests_Atomics : public NewParserTestsBase {
};

TEST_F(NewParserTests_Atomics, AtomicBarrierWithoutSpecifier) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  barrier\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Barrier", node->nodeName());
    ASSERT_EQ(AtomicNode::Ordering::SequentiallyConsistent, dynamic_cast<Three::BarrierNode*>(node)->ordering());
}

TEST_F(NewParserTests_Atomics, AtomicBarrierWithSpecifier) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  barrier:unordered\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Barrier", node->nodeName());
    ASSERT_EQ(AtomicNode::Ordering::None, dynamic_cast<Three::BarrierNode*>(node)->ordering());
}

TEST_F(NewParserTests_Atomics, AtomicExpressionAssign) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  atomic:ordered(a = a)\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Atomic Expression", node->nodeName());
    ASSERT_EQ(AtomicNode::Ordering::SequentiallyConsistent, dynamic_cast<Three::AtomicExpressionNode*>(node)->ordering());

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
}

TEST_F(NewParserTests_Atomics, AtomicStatement) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  atomic\n"
                                              "  end\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Atomic Statement", node->nodeName());
    ASSERT_TRUE(dynamic_cast<AtomicStatementNode*>(node)->elseClause() == nullptr);
}

TEST_F(NewParserTests_Atomics, AtomicStatementWithElse) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  atomic\n"
                                              "  else\n"
                                              "  end\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Atomic Statement", node->nodeName());
    ASSERT_TRUE(dynamic_cast<AtomicStatementNode*>(node)->elseClause() != nullptr);
}

TEST_F(NewParserTests_Atomics, AtomicStatementWithAbort) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  atomic\n"
                                              "    abort\n"
                                              "  end\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Atomic Statement", node->nodeName());
    ASSERT_EQ("Abort", node->childAtIndex(0)->nodeName());
}
