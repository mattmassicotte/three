#include "ParserTestBase.h"

class ParserTest_Atomics : public ParserTestBase {
};

TEST_F(ParserTest_Atomics, AtomicBarrierWithoutSpecifier) {
    Language::ASTNode* node;

    node = this->parse("def test()\nbarrier\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_EQ("Barrier", node->nodeName());
    ASSERT_EQ(AtomicNode::Ordering::SequentiallyConsistent, dynamic_cast<Language::BarrierNode*>(node)->ordering());
}

TEST_F(ParserTest_Atomics, AtomicBarrierWithSpecifier) {
    Language::ASTNode* node;

    node = this->parse("def test()\nbarrier:unordered\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_EQ("Barrier", node->nodeName());
    ASSERT_EQ(AtomicNode::Ordering::None, dynamic_cast<Language::BarrierNode*>(node)->ordering());
}

TEST_F(ParserTest_Atomics, AtomicExpression) {
    Language::ASTNode* node;

    node = this->parse("def test()\nInt a\natomic:ordered(a = 1)\nend\n");
    node = node->childAtIndex(0)->childAtIndex(1);

    ASSERT_EQ("AtomicExpression", node->nodeName());
    ASSERT_EQ(AtomicNode::Ordering::SequentiallyConsistent, dynamic_cast<Language::AtomicExpressionNode*>(node)->ordering());
}

TEST_F(ParserTest_Atomics, AtomicStatement) {
    Language::ASTNode* node;

    node = this->parse("def test()\natomic\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_EQ("AtomicStatement", node->name());
}
