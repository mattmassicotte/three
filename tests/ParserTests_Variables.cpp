#include "ParserTestBase.h"

class ParserTest_Variables : public ParserTestBase {
};

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
