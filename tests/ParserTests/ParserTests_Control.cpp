#include "../ParserTestsBase.h"

class ParserTests_Control : public ParserTestsBase {
};

TEST_F(ParserTests_Control, ReturnStatement) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  return\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Return", node->nodeName());
    ASSERT_EQ(0, node->childCount());
}

TEST_F(ParserTests_Control, ReturnStatementTailingIf) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  return if a\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("If", node->nodeName());
    ASSERT_EQ("Local Variable", dynamic_cast<IfNode*>(node)->condition()->nodeName());
    ASSERT_EQ(1, node->childCount());
    ASSERT_EQ("Return", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Control, ReturnOneExpression) {
    ASTNode* node = this->parseSingleFunction("def test(Int a) -> Int\n"
                                              "  return a + a\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Return", node->nodeName());
    ASSERT_EQ(1, node->childCount());
    ASSERT_EQ("Addition Operator", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Control, SimpleIfWithNoElseStatement) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  if a\n"
                                              "    a = a\n"
                                              "  end\n"
                                              "end\n");

    Three::IfNode* ifNode = dynamic_cast<Three::IfNode*>(node->childAtIndex(0));

    ASSERT_EQ("If", ifNode->nodeName());
    ASSERT_EQ("Local Variable", ifNode->condition()->nodeName());
    ASSERT_EQ(1, ifNode->childCount());
    ASSERT_EQ("Assign Operator", ifNode->childAtIndex(0)->nodeName());
    ASSERT_EQ(NULL, ifNode->elseStatement());
}

TEST_F(ParserTests_Control, SimpleIfWithElse) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  if a\n"
                                              "    a = a\n"
                                              "  else\n"
                                              "    a * a\n"
                                              "  end\n"
                                              "end\n");

    Three::IfNode* ifNode = dynamic_cast<Three::IfNode*>(node->childAtIndex(0));

    ASSERT_EQ("If", ifNode->nodeName());
    ASSERT_EQ("Local Variable", ifNode->condition()->nodeName());
    ASSERT_EQ(1, ifNode->childCount());
    ASSERT_EQ("Assign Operator", ifNode->childAtIndex(0)->nodeName());
    ASSERT_TRUE(ifNode->elseStatement() != nullptr);
    ASSERT_EQ(1, ifNode->elseStatement()->childCount());
    ASSERT_EQ("Multiplication Operator", ifNode->elseStatement()->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Control, SwitchWithElse) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  switch a\n"
                                              "  case a\n"
                                              "    return\n"
                                              "  case a\n"
                                              "    return\n"
                                              "  else\n"
                                              "    return\n"
                                              "  end\n"
                                              "end\n");

    SwitchNode* switchNode = dynamic_cast<SwitchNode*>(node->childAtIndex(0));

    ASSERT_EQ("Switch", switchNode->nodeName());

    ASSERT_EQ("Local Variable", switchNode->argument()->nodeName());
    ASSERT_EQ(2, switchNode->childCount());
    ASSERT_EQ("Case", switchNode->childAtIndex(0)->nodeName());
    ASSERT_EQ("Local Variable", dynamic_cast<CaseNode*>(switchNode->childAtIndex(0))->argument()->nodeName());
    ASSERT_EQ("Case", switchNode->childAtIndex(1)->nodeName());
    ASSERT_EQ("Local Variable", dynamic_cast<CaseNode*>(switchNode->childAtIndex(1))->argument()->nodeName());
    ASSERT_TRUE(switchNode->elseNode() != nullptr);
}
