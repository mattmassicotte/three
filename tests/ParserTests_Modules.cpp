#include "ParserTestBase.h"

class ParserTest_Modules : public ParserTestBase {
};

TEST_F(ParserTest_Modules, ImportLinkageAndInclude) {
    ASTNode* node = this->parse("import C.time\nlinkage(\"pthread\")\ninclude(\"pthread.h\")\n");

    ASSERT_EQ(3, node->childCount());

    ASSERT_IMPORT("C.time", node->childAtIndex(0));
    ASSERT_LINKAGE("pthread", node->childAtIndex(1));
    ASSERT_INCLUDE("pthread.h", node->childAtIndex(2));
}

TEST_F(ParserTest_Modules, IncludeFollowedByNewlines) {
    ASTNode* node = this->parse("include(\"pthread.h\")\n\n\ndef:type my_int Int\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_INCLUDE("pthread.h", node->childAtIndex(0));
    ASSERT_EQ("TypeDefinition", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTest_Modules, ModuleWithPublicType) {
    ASTNode* node = this->parse("public\ndef:type time_t Int\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("Visibility", node->childAtIndex(0)->nodeName());

    TypeDefinitionNode* typeDef = dynamic_cast<TypeDefinitionNode*>(node->childAtIndex(1));
    ASSERT_EQ("TypeDefinition", typeDef->nodeName());
    ASSERT_EQ(TranslationUnit::Visibility::External, typeDef->visibility());
    ASSERT_EQ(0, typeDef->sourceType().indirectionDepth());
    ASSERT_EQ("Int", typeDef->sourceType().name());
    ASSERT_EQ("time_t", typeDef->destTypeName());
}

TEST_F(ParserTest_Modules, FunctionDeclarationUsingDefinedType) {
    ASTNode* node = this->parse("public\ndef:func foo(Int a; Void)\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("FunctionDeclaration", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTest_Modules, FunctionDeclarationUsingDefinedStruct) {
    ASTNode* node = this->parse("struct MyStruct\nInt element\nend\ndef:func foo(MyStruct a)\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_STRUCT("MyStruct", 0, node->childAtIndex(0));
    ASSERT_EQ("FunctionDeclaration", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTest_Modules, DefTypeForFunction) {
    ASTNode* node = this->parse("def:type func_t (*Void; Int)\n");

    ASSERT_EQ(1, node->childCount());

    ASSERT_EQ("TypeDefinition", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTest_Modules, DefFuncWithEmptyFunctionAsParameter) {
    ASTNode* node = this->parse("def:func takes_func(() func)\n");

    ASSERT_EQ(1, node->childCount());

    ASSERT_EQ("FunctionDeclaration", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTest_Modules, DefValueWithoutValue) {
    ASTNode* node = this->parse("def:value abc\n");

    ASSERT_EQ(1, node->childCount());

    ASSERT_EQ("ValueDefinition", node->childAtIndex(0)->nodeName());
}
