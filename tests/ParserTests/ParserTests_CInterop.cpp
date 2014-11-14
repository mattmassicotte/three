#include "../ParserTestsBase.h"

class ParserTests_CInterop : public ParserTestsBase {
};

TEST_F(ParserTests_CInterop, UsingConstantMacroFromCHeader) {
    ASTNode* node = parseNodeWithBodies("include <stdio.h>\n\n"
                                        "def test()\n"
                                        "  Int x = EOF\n"
                                        "end\n");

    ASSERT_EQ(2, node->childCount());

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Variable Declaration", node->nodeName());

    VariableDeclarationNode* var = dynamic_cast<VariableDeclarationNode*>(node);
    ASSERT_EQ("x", var->name());
    ASSERT_EQ(DataType::Kind::Integer, var->dataType().kind());

    node = var->initializerExpression();
    ASSERT_EQ("C Macro", node->nodeName());
    ASSERT_EQ(DataType::Kind::CUnspecifiedMacro, node->dataType().kind());
}

TEST_F(ParserTests_CInterop, UsingTypeDefinedInCHeader) {
    ASTNode* node = parseNodeWithBodies("include <stdio.h>\n\n"
                                        "def test()\n"
                                        "  *FILE x = null\n"
                                        "end\n");

    ASSERT_EQ(2, node->childCount());

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Variable Declaration", node->nodeName());

    VariableDeclarationNode* var = dynamic_cast<VariableDeclarationNode*>(node);
    ASSERT_EQ("x", var->name());
    ASSERT_EQ(DataType::Kind::Pointer, var->dataType().kind());
    ASSERT_EQ(DataType::Kind::CStructure, var->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ("FILE", var->dataType().subtypeAtIndex(0).name());
}

TEST_F(ParserTests_CInterop, UsingTypedefTypeDefinedInCHeader) {
    ASTNode* node = parseNodeWithBodies("include <stdint.h>\n\n"
                                        "def test()\n"
                                        "  int_least8_t x = 0\n"
                                        "end\n");

    ASSERT_EQ(2, node->childCount());

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Variable Declaration", node->nodeName());

    VariableDeclarationNode* var = dynamic_cast<VariableDeclarationNode*>(node);
    ASSERT_EQ("x", var->name());
    ASSERT_EQ(DataType::Kind::CChar, var->dataType().kind());
    ASSERT_EQ("int_least8_t", var->dataType().name());
}

TEST_F(ParserTests_CInterop, UsingFunctionDefinedInHeader) {
    ASTNode* node = parseNodeWithBodies("include <stdio.h>\n\n"
                                        "def test()\n"
                                        "  printf(\"yo\")\n"
                                        "end\n");

    ASSERT_EQ(2, node->childCount());

    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Call Operator", node->nodeName());

    FunctionCallOperatorNode* fnCall = dynamic_cast<FunctionCallOperatorNode*>(node);

    ASSERT_EQ("Function Call Operator", fnCall->nodeName());
    ASSERT_EQ("Function Variable", fnCall->receiver()->nodeName());
    ASSERT_EQ("printf", fnCall->receiver()->name());
    ASSERT_EQ(1, fnCall->childCount());
}
