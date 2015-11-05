#include "../ParserTestsBase.h"

class ParserTests_Closures : public ParserTestsBase {
};

TEST_F(ParserTests_Closures, InlineClosureWithNoArguments) {
    ASTNode* node = this->parseNodeWithBodies("def foo({} c)\n"
                                              "end\n"
                                              "def test()\n"
                                              "  foo(do {})\n"
                                              "end\n");

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Call Operator", node->nodeName());

    ClosureNode* closure = dynamic_cast<ClosureNode*>(node->childAtIndex(0));
    ASSERT_EQ("Closure", closure->nodeName());
    ASSERT_EQ("test_closure_1", closure->name());
    ASSERT_EQ(DataType::Kind::Closure, closure->dataType().kind());
    ASSERT_EQ(0, closure->dataType().subtypeCount());
    ASSERT_EQ(1, closure->dataType().parameterCount());
    ASSERT_EQ(0, closure->dataType().returnCount());
}

TEST_F(ParserTests_Closures, TailingClosureWithNoArguments) {
    ASTNode* node = this->parseNodeWithBodies("def foo({} c)\n"
                                              "end\n"
                                              "def test()\n"
                                              "  foo() do {\n"
                                              "  }\n"
                                              "end\n");

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Call Operator", node->nodeName());

    ClosureNode* closure = dynamic_cast<ClosureNode*>(node->childAtIndex(0));
    ASSERT_EQ("Closure", closure->nodeName());
    ASSERT_EQ("test_closure_1", closure->name());
    ASSERT_EQ(DataType::Kind::Closure, closure->dataType().kind());
    ASSERT_EQ(0, closure->dataType().subtypeCount());
    ASSERT_EQ(1, closure->dataType().parameterCount());
    ASSERT_EQ(0, closure->dataType().returnCount());
}

TEST_F(ParserTests_Closures, InlineClosureWithOneArgument) {
    ASTNode* node = this->parseNodeWithBodies("def foo({} c)\n"
                                              "end\n"
                                              "def test()\n"
                                              "  foo(do (Int x) {})\n"
                                              "end\n");

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Call Operator", node->nodeName());

    ClosureNode* closure = dynamic_cast<ClosureNode*>(node->childAtIndex(0));
    ASSERT_EQ("Closure", closure->nodeName());
    ASSERT_EQ("test_closure_1", closure->name());
    ASSERT_EQ(DataType::Kind::Closure, closure->dataType().kind());
    ASSERT_EQ(0, closure->dataType().subtypeCount());
    ASSERT_EQ(2, closure->dataType().parameterCount());
    ASSERT_EQ(DataType::Kind::Integer, closure->dataType().parameterAtIndex(1).kind());
    ASSERT_EQ(DataType::Kind::Void, closure->dataType().returnType().kind());
}

TEST_F(ParserTests_Closures, InlineClosureWithTwoArguments) {
    ASTNode* node = this->parseNodeWithBodies("def foo({} c)\n"
                                              "end\n"
                                              "def test()\n"
                                              "  foo(do (Int x, Int y) {})\n"
                                              "end\n");

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Call Operator", node->nodeName());

    ClosureNode* closure = dynamic_cast<ClosureNode*>(node->childAtIndex(0));
    ASSERT_EQ("Closure", closure->nodeName());
    ASSERT_EQ(DataType::Kind::Closure, closure->dataType().kind());
    ASSERT_EQ(0, closure->dataType().subtypeCount());
    ASSERT_EQ(3, closure->dataType().parameterCount());
    ASSERT_EQ(DataType::Kind::Void, closure->dataType().returnType().kind());
}

TEST_F(ParserTests_Closures, ClosureVariableWithCapture) {
    ASTNode* node = this->parseNodeWithBodies("def test()\n"
                                              "  Int x = 42\n"
                                              "  Int y = 43\n"
                                              "  {Int} -> Int closure\n"
                                              "  closure = do (Int a; x) -> Int {\n"
                                              "    x = a + y\n"
                                              "  }\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Definition", node->nodeName());

    ASSERT_EQ("Variable Declaration", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("Variable Declaration", node->childAtIndex(1)->nodeName());
    ASSERT_EQ("Variable Declaration", node->childAtIndex(2)->nodeName());

    node = node->childAtIndex(3);
    ASSERT_EQ("Assign Operator", node->nodeName());

    ClosureNode* closure = dynamic_cast<ClosureNode*>(node->childAtIndex(1));
    ASSERT_EQ("Closure", closure->nodeName());
    EXPECT_EQ("test_closure_1", closure->name());
    EXPECT_EQ(DataType::Kind::Closure, closure->dataType().kind());
    EXPECT_EQ(0, closure->dataType().subtypeCount());
    EXPECT_EQ(2, closure->dataType().parameterCount());
    EXPECT_EQ(1, closure->dataType().returnCount());

    ASSERT_EQ(1, closure->childCount());

    node = closure->childAtIndex(0); // x = a + y
    ASSERT_EQ("Assign Operator", node->nodeName());

    EXPECT_EQ("Referenced Variable", node->childAtIndex(0)->nodeName());

    node = node->childAtIndex(1); // a + y
    ASSERT_EQ("Addition Operator", node->nodeName());
    EXPECT_EQ("Local Variable", node->childAtIndex(0)->nodeName());

    EXPECT_EQ("Captured Variable", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTests_Closures, NestedClosureVariableWithCapture) {
    ASTNode* node = this->parseNodeWithBodies("def test()\n"
                                              "  Int x = 42\n"
                                              "  Int y = 43\n"
                                              "  {} closure_a\n"
                                              "  closure_a = do (Int a; x) {\n"
                                              "    x = a\n"
                                              "    {} closure_b\n"
                                              "    closure_b = do (Int b; y) {\n"
                                              "      y = x\n"
                                              "      y = b\n"
                                              "    }\n"
                                              "  }\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Definition", node->nodeName());

    ASSERT_EQ("Variable Declaration", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("Variable Declaration", node->childAtIndex(1)->nodeName());
    ASSERT_EQ("Variable Declaration", node->childAtIndex(2)->nodeName());

    node = node->childAtIndex(3);
    ASSERT_EQ("Assign Operator", node->nodeName());

    ClosureNode* closure = dynamic_cast<ClosureNode*>(node->childAtIndex(1));
    ASSERT_EQ("Closure", closure->nodeName());
    EXPECT_EQ("test_closure_1", closure->name());
    EXPECT_EQ(DataType::Kind::Closure, closure->dataType().kind());
    EXPECT_EQ(0, closure->dataType().subtypeCount());
    EXPECT_EQ(2, closure->dataType().parameterCount());
    ASSERT_EQ(DataType::Kind::Void, closure->dataType().returnType().kind());

    ASSERT_EQ(3, closure->childCount());

    node = closure->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());

    EXPECT_EQ("Referenced Variable", node->childAtIndex(0)->nodeName());
    EXPECT_EQ("Local Variable", node->childAtIndex(1)->nodeName());

    node = closure->childAtIndex(2);
    ASSERT_EQ("Assign Operator", node->nodeName());

    closure = dynamic_cast<ClosureNode*>(node->childAtIndex(1));
    ASSERT_EQ("Closure", closure->nodeName());
    EXPECT_EQ("test_closure_2", closure->name());
    EXPECT_EQ(DataType::Kind::Closure, closure->dataType().kind());
    EXPECT_EQ(0, closure->dataType().subtypeCount());
    EXPECT_EQ(2, closure->dataType().parameterCount());
    ASSERT_EQ(DataType::Kind::Void, closure->dataType().returnType().kind());

    node = closure->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName()); // y = x

    EXPECT_EQ("Referenced Variable", node->childAtIndex(0)->nodeName());
    EXPECT_EQ("Captured Variable", node->childAtIndex(1)->nodeName());

    node = closure->childAtIndex(1);
    ASSERT_EQ("Assign Operator", node->nodeName()); // y = b

    EXPECT_EQ("Referenced Variable", node->childAtIndex(0)->nodeName());
    EXPECT_EQ("Local Variable", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTests_Closures, ClosureWithEmptyBody) {
    ASTNode* node = this->parseNodeWithBodies("def test()\n"
                                              "  {} closure = do () {\n"
                                              "  }\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Definition", node->nodeName());

    VariableDeclarationNode* varDecl = dynamic_cast<VariableDeclarationNode*>(node->childAtIndex(0));
    ASSERT_EQ("Variable Declaration", varDecl->nodeName());

    ASSERT_EQ("Closure", varDecl->initializerExpression()->nodeName());
}

TEST_F(ParserTests_Closures, TransposedClosure) {
    ASTNode* node = this->parseNodeWithBodies("def foo({} c)\n"
                                              "end\n"
                                              "def test()\n"
                                              "  transpose foo()\n"
                                              "end\n");

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());

    node = node->childAtIndex(0);
    ASSERT_EQ("Transpose Closure Node", node->nodeName());
    ASSERT_EQ(DataType::Kind::Void, node->dataType().kind());

    ASSERT_EQ("Function Call Operator", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Closures, AssignToTransposedClosure) {
    ASTNode* node = this->parseNodeWithBodies("def foo({} -> Int c)\n"
                                              "end\n"
                                              "def test()\n"
                                              "  Int x = transpose foo()\n"
                                              "end\n");

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());

    VariableDeclarationNode* varDecl = dynamic_cast<VariableDeclarationNode*>(node->childAtIndex(0));
    ASSERT_EQ("Variable Declaration", varDecl->nodeName());

    node = varDecl->initializerExpression();
    ASSERT_EQ("Transpose Closure Node", node->nodeName());
    ASSERT_EQ(DataType::Kind::Integer, node->dataType().kind());
}
