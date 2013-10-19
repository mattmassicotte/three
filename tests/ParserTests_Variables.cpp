#include "ParserTestBase.h"

class ParserTest_Variables : public ParserTestBase {
};

TEST_F(ParserTest_Variables, TopLevelVariableDeclaration) {
    ASTNode* node;

    node = this->parse("Int x\n");

    ASSERT_VARIABLE_DECLARATION("Int", 0, "x", node->childAtIndex(0));
}

TEST_F(ParserTest_Variables, TopLevelFunctionPointerVariableDeclaration) {
    ASTNode* node;

    node = this->parse("*(*Void arg; *Void) x\n");

    ASSERT_VARIABLE_DECLARATION("", 1, "x", node->childAtIndex(0));
}

TEST_F(ParserTest_Variables, NewLinesAroundTopLevelVariableDeclarations) {
    ASTNode* node;

    node = this->parse("Int x\n\nInt y\n");

    ASSERT_VARIABLE_DECLARATION("Int", 0, "x", node->childAtIndex(0));
    ASSERT_VARIABLE_DECLARATION("Int", 0, "y", node->childAtIndex(1));
}

TEST_F(ParserTest_Variables, TopLevelVariableUsedInFunction) {
    ASTNode* node;

    node = this->parse("Int x\ndef test(; Int)\nreturn x\nend\n");

    ASSERT_EQ(2, node->childCount());
    ASSERT_VARIABLE_DECLARATION("Int", 0, "x", node->childAtIndex(0));

    // get a reference to the return node
    node = node->childAtIndex(1)->childAtIndex(0);
    ASSERT_VARIABLE_NODE("Int", 0, "x", node->childAtIndex(0));
}

TEST_F(ParserTest_Variables, VariableDeclaration) {
    Three::ASTNode* node;

    node = this->parse("def test()\nInt x\nend\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLARATION("Int", 0, "x", node->childAtIndex(0));
}

TEST_F(ParserTest_Variables, IntegerVariableWithSpecifier) {
    Three::ASTNode* node;

    node = this->parse("def test()\nInt:64 x\nend\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLARATION("Int:64", 0, "x", node->childAtIndex(0));
}

TEST_F(ParserTest_Variables, VariableDeclarationWithInitializer) {
    Three::ASTNode* node;

    node = this->parse("def test()\nInt x = 0\nend\n");
    node = node->childAtIndex(0);

    VariableDeclarationNode* varNode = dynamic_cast<VariableDeclarationNode*>(node->childAtIndex(0));

    ASSERT_VARIABLE_DECLARATION("Int", 0, "x", varNode);
    ASSERT_INTEGER_LITERAL_NODE(0, varNode->initializerExpression());
}

TEST_F(ParserTest_Variables, ArrayDeclaration) {
    Three::ASTNode* node;

    node = this->parse("def test()\n[3]Int x\nend\n");
    node = node->childAtIndex(0);

    VariableDeclarationNode* varNode = dynamic_cast<VariableDeclarationNode*>(node->childAtIndex(0));

    ASSERT_VARIABLE_DECLARATION("Int", 0, "x", varNode);
    ASSERT_TRUE(varNode->variable()->type().isArray());
    ASSERT_EQ(3, varNode->variable()->type().arrayDimensions().at(0));
}
