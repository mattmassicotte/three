#include "ParserTestBase.h"

class ParserTest_Functions : public ParserTestBase {
};

TEST_F(ParserTest_Functions, ClosureInFunctionSignature) {
    Language::ASTNode* node;

    node = this->parse("def test({Int} closure)\nclosure(1)\nend\n");
    node = node->childAtIndex(0);
}

TEST_F(ParserTest_Functions, ClosureWithLabelledParameterInFunctionSignature) {
    Language::ASTNode* node;

    node = this->parse("def test({Int value} closure)\nclosure(1)\nend\n");
    node = node->childAtIndex(0);
}

TEST_F(ParserTest_Functions, MethodDefinition) {
    Language::ASTNode* node;

    node = this->parse("def Int.test()\nend\n");

    Language::FunctionDefinitionNode* defNode = dynamic_cast<Language::FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("FunctionDefinition", defNode->nodeName());
    ASSERT_EQ(0, defNode->childCount());
    ASSERT_EQ("test", defNode->function()->name());
    ASSERT_TRUE(defNode->function()->isPseudoMethod());
    ASSERT_DATA_TYPE("Int", defNode->function()->pseudoMethodType().referencedType());
    ASSERT_EQ("Int_3_test", defNode->function()->fullyQualifiedName());

    ASSERT_VARIABLE("Int", 1, "self", defNode->function()->parameterAtIndex(0));
    ASSERT_DATA_TYPE("Void", defNode->function()->returnType().referencedType());
}
