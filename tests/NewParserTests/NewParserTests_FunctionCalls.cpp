#include "../NewParserTestsBase.h"

class NewParserTests_FunctionCalls : public NewParserTestsBase {
};

TEST_F(NewParserTests_FunctionCalls, FunctionCallOperator) {
    ASTNode* node = this->parseSingleFunction("def test()\n"
                                              "  test()\n"
                                              "end\n");

    FunctionCallOperatorNode* fnCall = dynamic_cast<FunctionCallOperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Call Operator", fnCall->nodeName());
    ASSERT_EQ("Function Variable", fnCall->receiver()->nodeName());
    ASSERT_EQ(0, fnCall->childCount());
}

TEST_F(NewParserTests_FunctionCalls, FunctionCallOperatorWithArgument) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  test(a)\n"
                                              "end\n");

    FunctionCallOperatorNode* fnCall = dynamic_cast<FunctionCallOperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Call Operator", fnCall->nodeName());
    ASSERT_EQ(1, fnCall->childCount());
    ASSERT_EQ("Local Variable", fnCall->childAtIndex(0)->nodeName());
}

TEST_F(NewParserTests_FunctionCalls, FunctionCallOperatorWithTwoArguments) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  test(a, a)\n"
                                              "end\n");

    FunctionCallOperatorNode* fnCall = dynamic_cast<FunctionCallOperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Call Operator", fnCall->nodeName());
    ASSERT_EQ(2, fnCall->childCount());
    ASSERT_EQ("Local Variable", fnCall->childAtIndex(0)->nodeName());
    ASSERT_EQ("Local Variable", fnCall->childAtIndex(1)->nodeName());
}

TEST_F(NewParserTests_FunctionCalls, InvokeClosureParameter) {
    Three::ASTNode* node = this->parseSingleFunction("def test({Int} closure)\n"
                                                     "  closure(1)\n"
                                                     "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Call Operator", node->nodeName());

    Three::FunctionCallOperatorNode* fnCall = dynamic_cast<Three::FunctionCallOperatorNode*>(node);

    ASSERT_EQ(1, fnCall->childCount());
    ASSERT_EQ("Local Variable", fnCall->receiver()->nodeName());
}

TEST_F(NewParserTests_FunctionCalls, InvokeClosurePointerParameter) {
    Three::ASTNode* node = this->parseSingleFunction("def test(*{Int} closure)\n"
                                                     "  (*closure)(1)\n"
                                                     "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Call Operator", node->nodeName());

    Three::FunctionCallOperatorNode* fnCall = dynamic_cast<Three::FunctionCallOperatorNode*>(node);

    ASSERT_EQ(1, fnCall->childCount());

    node = fnCall->receiver();
    ASSERT_EQ("Dereference Operator", node->nodeName());
    ASSERT_EQ("Local Variable", node->childAtIndex(0)->nodeName());
}

TEST_F(NewParserTests_FunctionCalls, MethodInvocation) {
    Three::ASTNode* node = this->parseNodeWithBodies("def Int.test(Int b)\n"
                                                     "end\n"
                                                     "def invoke(*Int a)\n"
                                                     "  a.test(5)\n"
                                                     "end\n");

    ASSERT_EQ(2, node->childCount());
    ASSERT_EQ("Function Definition", node->childAtIndex(0)->nodeName());
    ASSERT_EQ(NewDataType::Kind::Integer, dynamic_cast<Three::FunctionDefinitionNode*>(node->childAtIndex(0))->methodOnType().kind());

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Method Call Operator", node->nodeName());

    Three::MethodCallOperatorNode* method = dynamic_cast<Three::MethodCallOperatorNode*>(node);

    ASSERT_EQ("test", method->name());
    ASSERT_EQ("Local Variable", method->receiver()->nodeName());
    ASSERT_EQ("a", dynamic_cast<Three::VariableNode*>(method->receiver())->name());
    ASSERT_EQ(1, method->childCount());
    EXPECT_EQ("Integer Literal", method->childAtIndex(0)->nodeName());
}

TEST_F(NewParserTests_FunctionCalls, InvokeVariableAsFunction) {
    Three::ASTNode* node = this->parseSingleFunction("def test()\n"
                                                     "  (Int) func\n"
                                                     "  func(1)\n"
                                                     "end\n");

    ASSERT_EQ(2, node->childCount());
    EXPECT_EQ("Variable Declaration", node->childAtIndex(0)->nodeName());

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Call Operator", node->nodeName());

    Three::FunctionCallOperatorNode* fnCall = dynamic_cast<Three::FunctionCallOperatorNode*>(node);

    ASSERT_EQ("Local Variable", fnCall->receiver()->nodeName());
}

TEST_F(NewParserTests_FunctionCalls, FunctionCallDereference) {
    Three::ASTNode* node = this->parseNodeWithBodies("def foo(;*Int)\n"
                                                     "  return null\n"
                                                     "end\n"
                                                     "def test()\n"
                                                     "  *foo() = 1\n"
                                                     "end\n");

    ASSERT_EQ(2, node->childCount());
    ASSERT_EQ("Function Definition", node->childAtIndex(0)->nodeName());

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());

    node = node->childAtIndex(0);
    ASSERT_EQ("Dereference Operator", node->nodeName());
    ASSERT_EQ("Function Call Operator", node->childAtIndex(0)->nodeName());
}