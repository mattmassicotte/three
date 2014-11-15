#include "../ParserTestsBase.h"

class ParserTests_Atomics : public ParserTestsBase {
};

TEST_F(ParserTests_Atomics, AtomicBarrierWithoutSpecifier) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  barrier\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Barrier", node->nodeName());
    ASSERT_EQ(AtomicNode::Ordering::SequentiallyConsistent, dynamic_cast<Three::BarrierNode*>(node)->ordering());
}

TEST_F(ParserTests_Atomics, AtomicBarrierWithSpecifier) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  barrier:unordered\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Barrier", node->nodeName());
    ASSERT_EQ(AtomicNode::Ordering::None, dynamic_cast<Three::BarrierNode*>(node)->ordering());
}

TEST_F(ParserTests_Atomics, AtomicExpressionAssign) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  atomic:ordered(a = a)\n"
                                              "end\n");

    ASSERT_EQ(1, node->childCount());
    node = node->childAtIndex(0);

    ASSERT_EQ("Atomic Expression", node->nodeName());
    ASSERT_EQ(AtomicNode::Ordering::SequentiallyConsistent, dynamic_cast<Three::AtomicExpressionNode*>(node)->ordering());

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
}

TEST_F(ParserTests_Atomics, AtomicStatement) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  atomic\n"
                                              "  end\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Atomic Statement", node->nodeName());
    ASSERT_TRUE(dynamic_cast<AtomicStatementNode*>(node)->elseClause() == nullptr);
}

TEST_F(ParserTests_Atomics, AtomicStatementWithElse) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  atomic\n"
                                              "  else\n"
                                              "  end\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Atomic Statement", node->nodeName());
    ASSERT_TRUE(dynamic_cast<AtomicStatementNode*>(node)->elseClause() != nullptr);
}

TEST_F(ParserTests_Atomics, AtomicStatementWithAbort) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  atomic\n"
                                              "    abort\n"
                                              "  end\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Atomic Statement", node->nodeName());
    ASSERT_EQ("Abort", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Atomics, AtomicStatementWithFallbackFunctions) {
    ASTNode* node = this->parseNodeWithBodies("def fn_1(*Void ptr; Bool)\n"
                                              "  return true\n"
                                              "end\n"
                                              "def fn_2(*Void ptr; Bool)\n"
                                              "  return true\n"
                                              "end\n"
                                              "def test(*Void ptr)\n"
                                              "  atomic:fallback(fn_1, fn_2, ptr)\n"
                                              "    abort\n"
                                              "  end\n"
                                              "end\n");

    ASSERT_EQ(3, node->childCount());
    node = node->childAtIndex(2);

    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(1, node->childCount());
    node = node->childAtIndex(0);

    ASSERT_EQ("Atomic Statement", node->nodeName());
    auto atomicNode = dynamic_cast<AtomicStatementNode*>(node);

    node = atomicNode->lockFunction();
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ("Function Variable", node->nodeName());
    ASSERT_EQ("fn_1", node->name());

    node = atomicNode->unlockFunction();
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ("Function Variable", node->nodeName());
    ASSERT_EQ("fn_2", node->name());

    node = atomicNode->lockContext();
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ("Local Variable", node->nodeName());
    ASSERT_EQ("ptr", node->name());
}
