#include "ParserTestBase.h"

class ParserTest_Functions : public ParserTestBase {
};

TEST_F(ParserTest_Functions, EmptyFunction) {
    Language::ASTNode* node;

    node = this->parse("def test()\nend\n");

    ASSERT_FUNCTION_DEFINITION("test", node->childAtIndex(0));
}

TEST_F(ParserTest_Functions, BackToBackEmptyFunctions) {
    Language::ASTNode* node;

    node = this->parse("def test()\nend\ndef test2()\nend\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_FUNCTION_DEFINITION("test", node->childAtIndex(0));
    ASSERT_FUNCTION_DEFINITION("test2", node->childAtIndex(1));
}

TEST_F(ParserTest_Functions, ClosureInFunctionSignature) {
    Language::ASTNode* node;

    node = this->parse("def test({Int} closure)\nclosure(1)\nend\n");
    node = node->childAtIndex(0);

    ASSERT_FUNCTION_DEFINITION("test", node);
    // TODO
}

TEST_F(ParserTest_Functions, ClosureWithLabelledParameterInFunctionSignature) {
    Language::ASTNode* node;

    node = this->parse("def test({Int value} closure)\nclosure(1)\nend\n");
    node = node->childAtIndex(0);

    ASSERT_FUNCTION_DEFINITION("test", node);
    // TODO
}

TEST_F(ParserTest_Functions, MethodDefinition) {
    Language::ASTNode* node;

    node = this->parse("def Int.test()\nend\n");

    Language::FunctionDefinitionNode* defNode = dynamic_cast<Language::FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_FUNCTION_DEFINITION("Int_3_test", defNode);
    ASSERT_EQ(0, defNode->childCount());
    ASSERT_EQ("test", defNode->function()->name());
    ASSERT_TRUE(defNode->function()->isPseudoMethod());
    ASSERT_DATA_TYPE("Int", defNode->function()->pseudoMethodType().referencedType());

    ASSERT_VARIABLE("Int", 1, "self", defNode->function()->parameterAtIndex(0));
    ASSERT_DATA_TYPE("Void", defNode->function()->returnType().referencedType());
}

TEST_F(ParserTest_Functions, MethodInvocation) {
    Language::ASTNode* node;

    node = this->parse("def Int.test(Int b)\nend\n def invoke(*Int a)\n a.test(5)\nend\n");
    ASSERT_EQ(2, node->childCount());

    // grab a reference to the method invocation
    Language::FunctionCallNode* callNode;

    callNode = dynamic_cast<Language::FunctionCallNode*>(node->childAtIndex(1)->childAtIndex(0));

    ASSERT_EQ("FunctionCall", callNode->name());
    ASSERT_EQ("Int_3_test", callNode->functionName());
    ASSERT_EQ(2, callNode->childCount());
    ASSERT_VARIABLE_NODE("Int",  1, "a", callNode->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(5, callNode->childAtIndex(1));
}

TEST_F(ParserTest_Functions, SelfInMethodDefinition) {
    Language::ASTNode* node;

    node = this->parse("def Int.test()\n*self + 1\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("+", node);
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));

    node = node->childAtIndex(0);
    ASSERT_OPERATOR("*", node);
    ASSERT_VARIABLE_NODE("Int",  1, "self", node->childAtIndex(0));
}
