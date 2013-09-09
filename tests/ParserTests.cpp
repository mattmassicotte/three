#include "ParserTestBase.h"

class ParserTest : public ParserTestBase {
};

TEST_F(ParserTest, SimpleHelloWorldProgram) {
    Language::ASTNode* node;

    node = this->parse("include \"stdio.h\"\n\ndef main(Int argc, **Char argv; Int)\n    printf(\"hello world\")\n    return 0\nend\n");

    ASSERT_EQ("Root", node->name());
    ASSERT_EQ(2, node->childCount());

    // include statement
    ASSERT_INCLUDE("stdio.h", node->childAtIndex(0));

    // function definition
    Language::FunctionDefinitionNode* defNode;

    defNode = dynamic_cast<Language::FunctionDefinitionNode*>(node->childAtIndex(1));

    ASSERT_EQ("FunctionDefinition", defNode->name());
    ASSERT_EQ(2, defNode->childCount());
    ASSERT_EQ("main", defNode->function()->name());

    ASSERT_VARIABLE("Int",  0, "argc", defNode->function()->parameterAtIndex(0));
    ASSERT_VARIABLE("Char", 2, "argv", defNode->function()->parameterAtIndex(1));
    // TODO: fix this
    // ASSERT_DATA_TYPE("Int",  0, defNode->function()->returnType());

    Language::FunctionCallNode* callNode;

    callNode = dynamic_cast<Language::FunctionCallNode*>(defNode->childAtIndex(0));

    ASSERT_EQ("FunctionCall", callNode->name());
    ASSERT_EQ("printf", callNode->functionName());
    ASSERT_EQ(1, callNode->childCount());

    ASSERT_STRING_LITERAL_NODE("hello world", callNode->childAtIndex(0));

    // return statement
    Language::ReturnNode* returnNode = dynamic_cast<Language::ReturnNode*>(defNode->childAtIndex(1));

    ASSERT_RETURN_NODE(returnNode);
    ASSERT_EQ(1, returnNode->childCount());
    ASSERT_INTEGER_LITERAL_NODE(0, returnNode->childAtIndex(0));
}

TEST_F(ParserTest, SimpleIfWithNoElseStatement) {
    Language::ASTNode* node;

    node = this->parse("def test()\nif true\nreturn\nend\nend\n");

    Language::IfNode* ifNode = dynamic_cast<Language::IfNode*>(node->childAtIndex(0)->childAtIndex(0));

    ASSERT_EQ("If", ifNode->name());
    ASSERT_BOOLEAN_LITERAL_NODE(true, ifNode->condition());
    ASSERT_EQ(1, ifNode->childCount());
    ASSERT_RETURN_NODE(ifNode->childAtIndex(0));
    ASSERT_EQ(NULL, ifNode->elseStatement());
}

TEST_F(ParserTest, SimpleIfWithElseStatement) {
    Language::ASTNode* node;

    node = this->parse("def test()\nif false\nreturn\nelse\nreturn\nend\nend\n");

    Language::IfNode* ifNode = dynamic_cast<Language::IfNode*>(node->childAtIndex(0)->childAtIndex(0));

    ASSERT_EQ("If", ifNode->name());
    ASSERT_BOOLEAN_LITERAL_NODE(false, ifNode->condition());
    ASSERT_EQ(1, ifNode->childCount());
    ASSERT_RETURN_NODE(ifNode->childAtIndex(0));
    ASSERT_RETURN_NODE(ifNode->elseStatement()->childAtIndex(0));
}

TEST_F(ParserTest, TailingIfStatement) {
    Language::ASTNode* node;

    node = this->parse("def test()\nreturn if false\nend\n");

    Language::IfNode* ifNode = dynamic_cast<Language::IfNode*>(node->childAtIndex(0)->childAtIndex(0));

    ASSERT_EQ("If", ifNode->name());
    ASSERT_BOOLEAN_LITERAL_NODE(false, ifNode->condition());
    ASSERT_EQ(1, ifNode->childCount());
    ASSERT_RETURN_NODE(ifNode->childAtIndex(0));
}

TEST_F(ParserTest, ClosureInFunction) {
    Language::ASTNode* node;

    node = this->parse("include \"stdio.h\"\n\ndef main(;Int)\nInt x\nInt y\n{Int, Int; Int} closure\nx = 42\ny = 43\nclosure = do (Int a, Int b; Int; x) {\nx = a + b + y\nreturn a + b\n}\n printf(\"x = %d, value = %d\\n\", x, closure(1,2))\nprintf(\"x = %d\\n\", x)\nend\n");

    // include statement
    ASSERT_INCLUDE("stdio.h", node->childAtIndex(0));

    node = node->childAtIndex(1);

    // Variable declarations
    ASSERT_VARIABLE_DECLERATION("Int", 0, "x", node->childAtIndex(0));
    ASSERT_VARIABLE_DECLERATION("Int", 0, "y", node->childAtIndex(1));

    Language::VariableDeclarationNode* varDecl = dynamic_cast<Language::VariableDeclarationNode*>(node->childAtIndex(2));
    ASSERT_VARIABLE_DECLERATION("", 0, "closure", varDecl);
    ASSERT_EQ(2, varDecl->variable()->type().referencedType()->childCount());
    ASSERT_EQ("Int", varDecl->variable()->type().referencedType()->childAtIndex(0).name());
    ASSERT_EQ(0, varDecl->variable()->type().referencedType()->childAtIndex(0).indirectionDepth());
    ASSERT_EQ("Int", varDecl->variable()->type().referencedType()->childAtIndex(1).name());
    ASSERT_EQ(0, varDecl->variable()->type().referencedType()->childAtIndex(1).indirectionDepth());

    ASSERT_OPERATOR("=", node->childAtIndex(3));
    ASSERT_OPERATOR("=", node->childAtIndex(4));
    ASSERT_OPERATOR("=", node->childAtIndex(5));

    Language::ClosureNode* closure = dynamic_cast<Language::ClosureNode*>(node->childAtIndex(5)->childAtIndex(1));
    ASSERT_EQ("Closure", closure->name());
}

TEST_F(ParserTest, NewlinesAfterStatements) {
    Language::ASTNode* node;

    node = this->parse("def test()\n\nInt x\n\nx = 1\n\n\nend\n\n\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLERATION("Int", 0, "x", node->childAtIndex(0));

    node = node->childAtIndex(1);

    ASSERT_OPERATOR("=", node);
    ASSERT_VARIABLE_NODE("Int",  0, "x", node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
}

TEST_F(ParserTest, StructureDefinitionWithPacking) {
    Language::ASTNode* node;

    node = this->parse("struct:2 MyStructure\nInt x\nInt y\nend\n");

    ASSERT_STRUCT("MyStructure", 2, node->childAtIndex(0));
}

TEST_F(ParserTest, StructureDefinitionWithoutPacking) {
    Language::ASTNode* node;

    node = this->parse("struct MyStructure\nInt x\nInt y\nend\n");

    ASSERT_STRUCT("MyStructure", 0, node->childAtIndex(0));
}

TEST_F(ParserTest, StructureDefinitionUsedAsType) {
    Language::ASTNode* node;

    node = this->parse("struct MyStructure\nInt x\nend\ndef foo()\nMyStructure a\nend\n");

    ASSERT_STRUCT("MyStructure", 0, node->childAtIndex(0));

    node = node->childAtIndex(1);

    ASSERT_VARIABLE_DECLERATION("MyStructure",  0, "a", node->childAtIndex(0));
}

TEST_F(ParserTest, AtomicBarrierWithoutSpecifier) {
    Language::ASTNode* node;

    node = this->parse("def test()\nbarrier\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_EQ("Barrier", node->name());
    ASSERT_EQ("ordered", dynamic_cast<Language::BarrierNode*>(node)->type());
}

TEST_F(ParserTest, AtomicBarrierWithSpecifier) {
    Language::ASTNode* node;

    node = this->parse("def test()\nbarrier:unordered\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_EQ("Barrier", node->name());
    ASSERT_EQ("unordered", dynamic_cast<Language::BarrierNode*>(node)->type());
}

TEST_F(ParserTest, AtomicExpression) {
    Language::ASTNode* node;

    node = this->parse("def test()\nInt a\natomic:ordered(a = 1)\nend\n");
    node = node->childAtIndex(0)->childAtIndex(1);

    ASSERT_EQ("AtomicExpression", node->name());
    ASSERT_EQ("ordered", dynamic_cast<Language::AtomicExpressionNode*>(node)->type());
}

TEST_F(ParserTest, AtomicStatement) {
    Language::ASTNode* node;

    node = this->parse("def test()\natomic\nend\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_EQ("AtomicStatement", node->name());
}

TEST_F(ParserTest, NullLiteral) {
    Language::ASTNode* node;

    node = this->parse("def test(*Int a)\na = null\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_EQ("NullLiteral", node->childAtIndex(1)->name());
}

TEST_F(ParserTest, BinaryIntegerLiteral) {
    Language::ASTNode* node;

    node = this->parse("def test(Int a)\na = 0b1001\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_INTEGER_LITERAL_NODE(9, node->childAtIndex(1));
}

TEST_F(ParserTest, OrAssignmentOperator) {
    Language::ASTNode* node;

    node = this->parse("def test(Int a)\na ||= 1\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("||=", node);
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
}

TEST_F(ParserTest, Comment) {
    Language::ASTNode* node;

    node = this->parse("def test()\n//Int a = 1\nreturn\nend\n");
    node = node->childAtIndex(0);

    ASSERT_EQ(1, node->childCount());
    ASSERT_RETURN_NODE(node->childAtIndex(0));
}

TEST_F(ParserTest, PointerVariableDefinition) {
    Language::ASTNode* node;

    node = this->parse("def test()\n*Char a\nend\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLERATION("Char",  1, "a", node->childAtIndex(0));
}

TEST_F(ParserTest, MultiplePointerVariableDefinition) {
    Language::ASTNode* node;

    node = this->parse("def test()\n***Char a\nend\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLERATION("Char",  3, "a", node->childAtIndex(0));
}

TEST_F(ParserTest, FunctionCallDereference) {
    Language::ASTNode* node;

    node = this->parse("def foo(;*Int)\nreturn null\nend\ndef test()\n*foo() = 1\nend\n");
    node = node->childAtIndex(0);
}

TEST_F(ParserTest, InlineClosureWithoutTypes) {
    Language::ASTNode* node;

    node = this->parse("def foo({} c)\nend\ndef test()\nfoo(do {})\nend\n");
    node = node->childAtIndex(1); // def test
    node = node->childAtIndex(0); // foo(..)
    node = node->childAtIndex(0);

    ASSERT_EQ("Closure", node->name());
}

TEST_F(ParserTest, InlineClosureWithTypes) {
    Language::ASTNode* node;

    node = this->parse("def foo({Int} c)\nend\ndef test()\nfoo(do (Int x) {})\nend\n");
    node = node->childAtIndex(1); // def test
    node = node->childAtIndex(0); // foo(..)
    node = node->childAtIndex(0);

    ASSERT_EQ("Closure", node->name());
}
