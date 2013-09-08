#include "ParserTestBase.h"

class ParserTest_Variables : public ParserTestBase {
};

TEST_F(ParserTest_Variables, TopLevelVariableDeclaration) {
    Language::ASTNode* node;

    node = this->parse("Int x\n");

    ASSERT_VARIABLE_DECLERATION("Int", 0, "x", node->childAtIndex(0));
}

TEST_F(ParserTest_Variables, NewLinesAroundTopLevelVariableDeclarations) {
    Language::ASTNode* node;

    node = this->parse("Int x\n\nInt y\n");

    ASSERT_VARIABLE_DECLERATION("Int", 0, "x", node->childAtIndex(0));
    ASSERT_VARIABLE_DECLERATION("Int", 0, "y", node->childAtIndex(1));
}

TEST_F(ParserTest_Variables, VariableDeclaration) {
    Language::ASTNode* node;

    node = this->parse("def test()\nInt x\nend\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLERATION("Int", 0, "x", node->childAtIndex(0));
}

TEST_F(ParserTest_Variables, IntegerVariableWithSpecifier) {
    Language::ASTNode* node;

    node = this->parse("def test()\nInt:64 x\nend\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLERATION("Int:64", 0, "x", node->childAtIndex(0));
}

TEST_F(ParserTest_Variables, VariableDeclarationWithInitializer) {
    Language::ASTNode* node;

    node = this->parse("def test()\nInt x = 0\nend\n");
    node = node->childAtIndex(0);

    VariableDeclarationNode* varNode = dynamic_cast<VariableDeclarationNode*>(node->childAtIndex(0));

    ASSERT_VARIABLE_DECLERATION("Int", 0, "x", varNode);
    ASSERT_INTEGER_LITERAL_NODE(0, varNode->initializerExpression());
}

// TEST_F(ParserTest_Variables, ArrayDeclaration) {
//     Language::ASTNode* node;
// 
//     node = this->parse("def test()\n[3]Int x\nend\n");
//     node = node->childAtIndex(0);
// 
//     VariableDeclarationNode* varNode = dynamic_cast<VariableDeclarationNode*>(node->childAtIndex(0));
// 
//     ASSERT_VARIABLE_DECLERATION("Int", 0, "x", varNode);
//     ASSERT_TRUE(varNode->variable()->type().isArray());
//     ASSERT_EQ(5, varNode->variable()->type().arrayDimensions().at(0));
// }
