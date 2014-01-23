#include "../ParserTestBase.h"

class ParserTest : public ParserTestBase {
};

TEST_F(ParserTest, SimpleHelloWorldProgram) {
    Three::ASTNode* node;

    node = this->parse("include \"stdio.h\"\n\ndef main(Int argc, **Char argv; Int)\n    printf(\"hello world\")\n    return 0\nend\n");

    ASSERT_EQ("Root", node->name());
    ASSERT_EQ(2, node->childCount());

    // include statement
    ASSERT_INCLUDE("stdio.h", node->childAtIndex(0));

    // function definition
    Three::FunctionDefinitionNode* defNode;

    defNode = dynamic_cast<Three::FunctionDefinitionNode*>(node->childAtIndex(1));

    ASSERT_EQ("FunctionDefinition", defNode->name());
    ASSERT_EQ(2, defNode->childCount());
    ASSERT_EQ("main", defNode->function()->name());

    ASSERT_VARIABLE("Int",  0, "argc", defNode->function()->parameterAtIndex(0));
    ASSERT_VARIABLE("Char", 2, "argv", defNode->function()->parameterAtIndex(1));
    // TODO: fix this
    // ASSERT_DATA_TYPE("Int",  0, defNode->function()->returnType());

    node = defNode->childAtIndex(0);
    ASSERT_OPERATOR("()", node);
    // ASSERT_FUNCTION_CALL("printf", node);
    ASSERT_EQ(1, node->childCount());

    ASSERT_STRING_LITERAL_NODE("hello world", node->childAtIndex(0));

    // return statement
    node = defNode->childAtIndex(1);

    ASSERT_RETURN_NODE(node);
    ASSERT_EQ(1, node->childCount());
    ASSERT_INTEGER_LITERAL_NODE(0, node->childAtIndex(0));
}

TEST_F(ParserTest, NewlinesAfterStatements) {
    Three::ASTNode* node;

    node = this->parse("def test()\n\nInt x\n\nx = 1\n\n\nend\n\n\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLARATION("Int", 0, "x", node->childAtIndex(0));

    node = node->childAtIndex(1);

    ASSERT_OPERATOR("=", node);
    ASSERT_VARIABLE_NODE("Int",  0, "x", node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
}

TEST_F(ParserTest, NullLiteral) {
    Three::ASTNode* node;

    node = this->parse("def test(*Int a)\na = null\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_EQ("NullLiteral", node->childAtIndex(1)->name());
}

TEST_F(ParserTest, BinaryIntegerLiteral) {
    Three::ASTNode* node;

    node = this->parse("def test(Int a)\na = 0b1001\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_INTEGER_LITERAL_NODE(9, node->childAtIndex(1));
}

TEST_F(ParserTest, Comment) {
    Three::ASTNode* node;

    node = this->parse("def test()\n# Int a = 1\nreturn\nend\n");
    node = node->childAtIndex(0);

    ASSERT_EQ(1, node->childCount());
    ASSERT_RETURN_NODE(node->childAtIndex(0));
}
