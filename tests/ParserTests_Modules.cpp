#include "ParserTestBase.h"

class ParserTest_Modules : public ParserTestBase {
};

TEST_F(ParserTest_Modules, BasicModule) {
    ASTNode* node = this->parse("module MyModule\nend\n");

    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_MODULE("MyModule", node);
}

TEST_F(ParserTest_Modules, ModuleWithPublicType) {
    ASTNode* node = this->parse("module MyModule\npublic\ndef:type time_t Int\nend\n");
    node = node->childAtIndex(0); // this is the module

    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("Visibility", node->childAtIndex(0)->nodeName());

    TypeDefinitionNode* typeDef = dynamic_cast<TypeDefinitionNode*>(node->childAtIndex(1));
    ASSERT_EQ("TypeDefinition", typeDef->nodeName());
    ASSERT_EQ(TranslationUnit::Visibility::External, typeDef->visibility());
    ASSERT_EQ(0, typeDef->sourceType().indirectionDepth());
    ASSERT_EQ("Int", typeDef->sourceType().name());
    ASSERT_EQ("time_t", typeDef->destTypeName());
}

TEST_F(ParserTest_Modules, ModuleWithImportLinkageInclude) {
    ASTNode* node = this->parse("module C.threads\nimport C.time\nlinkage(\"pthread\")\ninclude(\"pthread.h\")\nend\n");
    node = node->childAtIndex(0); // this is the module

    ASSERT_MODULE("C.threads", node);
    ASSERT_EQ(3, node->childCount());

    ASSERT_IMPORT("C.time", node->childAtIndex(0));
    ASSERT_LINKAGE("pthread", node->childAtIndex(1));
    ASSERT_INCLUDE("pthread.h", node->childAtIndex(2));
}

TEST_F(ParserTest_Modules, ModuleWithStruct) {
    ASTNode* node = this->parse("module HasStruct\nstruct MyStruct\nInt element\nend\nend\n");
    node = node->childAtIndex(0); // this is the module

    ASSERT_EQ(1, node->childCount());

    ASSERT_STRUCT("MyStruct", 0, node->childAtIndex(0));
}

TEST_F(ParserTest_Modules, ModuleWithFunctionDeclaration) {
    ASTNode* node = this->parse("module HasFunction\npublic\ndef printf(@distinct * @const Char format; Int)\nend\n");
    node = node->childAtIndex(0);

    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("FunctionDeclaration", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTest_Modules, ModuleWithFunctionDeclarationUsingBuiltinType) {
    ASTNode* node = this->parse("module HasFunction\npublic\ndef foo(Int a; Void)\nend\n");
    node = node->childAtIndex(0);

    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("FunctionDeclaration", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTest_Modules, ModuleWithStructUsedInFunctionDeclaration) {
    ASTNode* node = this->parse("module HasStruct\nstruct MyStruct\nInt element\nend\ndef foo(MyStruct a)\nend\n");
    node = node->childAtIndex(0); // this is the module

    ASSERT_EQ(2, node->childCount());

    ASSERT_STRUCT("MyStruct", 0, node->childAtIndex(0));
    ASSERT_EQ("FunctionDeclaration", node->childAtIndex(1)->nodeName());
}
